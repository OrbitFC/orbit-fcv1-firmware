#include "crc.h"
#include "rng.h"
#include "tcp.h"
#include "stm32h7xx_hal_rng.h"
#include "task_semphr.h"


SemaphoreHandle_t crcMutex;
struct tcpTypeDef htcp;

uint32_t tcpOpen(void)
{
    if (htcp.state == TCP_STATE_CLOSED) {
        htcp.state = TCP_STATE_LISTEN;
        return TCP_OK;
    } else
        return TCP_WARNING_UNCHANGED;   
}

uint32_t tcpConnect(uint8_t src, uint8_t dest, uint32_t key)
{
    // [TODO] Handle edge cases
    if ((htcp.state == TCP_STATE_CLOSED) || (htcp.state == TCP_STATE_LISTEN)) {
        uint32_t rnd;
        xSemaphoreTake(rngMutex, portMAX_DELAY);
        HAL_RNG_GenerateRandomNumber(&hrng, &rnd);
        xSemaphoreGive(rngMutex);
        empty_packet_header->sequence_number = rnd & 0xffff;
        empty_packet_header->ack_number = (rnd & 0xffff0000) >> 16;
        empty_packet_header->source_port = src;
        empty_packet_header->dest_port = dest;
        empty_packet_header->window = MAX_WINDOW;
        empty_packet_header->packet_len = 0;
        empty_packet_header->flag = 1 << SYN;
        empty_packet_header->key = key;
        __tcp_transmit(empty_packet, 0);

        htcp.state = TCP_STATE_SYN_SENT;
        uint32_t _r;
        while (1) {     // [TODO] Implement timer to terminate waiting
            struct tcp_header* header;
            uint32_t crc;
            _r = __tcp_receive_packet(&header, &crc);
            if (_r != TCP_FAILED) {
                if ((header->ack_number != (empty_packet_header->sequence_number + 1)) || (header->sequence_number != empty_packet_header->ack_number))
                    continue;   // Incorrect acknowledge number. TCP do not allow out-of-order when establish connection.
                if (header->flag & ((1 << SYN) | (1 << ACK))) {
                    htcp.sequence_number = empty_packet_header->sequence_number++;
                    htcp.ack_number = empty_packet_header->ack_number++;
                    empty_packet_header->flag = 1 << ACK;
                    __tcp_transmit(empty_packet, 0);
                    htcp.state = TCP_STATE_ESTABLISHED;
                    return TCP_OK;
                } else { // Incorrect flag
                    continue;
                }
            } else {
                continue;
            }
        }
    } else {
        return TCP_WARNING_UNCHANGED;
    }
}

uint32_t tcpRxHandler(uint32_t d)
{
    uint32_t crc_1, crc;
    // [TODO] Replace buf with static variable
    // Only works with little endian system
    // [TODO] RX_STATE_WAIT_PACKET and RX_STATE_ESTABLISHED to reject packet with incorrect flag (such as SYN when already have an established connection) and other general manipulation of TCP state
    switch (htcp.rx_state) {
        case RX_STATE_CLOSED:   // Ignore
            break;
        case RX_STATE_ESTABLISHED:
            __tcp_parse_header(d);
            htcp.rx_state = RX_STATE_WAIT_PACKET;
            break;
        case RX_STATE_WAIT_PACKET:
            __tcp_parse_header(d);
            if (htcp.rx_count == HEADER_SIZE_WORD) {
                tcp_header_data[packet_data_offset_w] = m_header;
                htcp.rx_state = RX_STATE_WAIT_DATA;
                packet_data_offset[packet_data_offset_w++] = packet_data_w;
            }
            break;
        case RX_STATE_WAIT_DATA:
            // Primitive Data handler
            // [TODO] Implement faster memory transfer
            packet_data[packet_data_w++] = d;
            if ((++htcp.rx_count) == (m_header.packet_len + HEADER_SIZE_WORD)) {
                htcp.rx_state = RX_STATE_WAIT_CRC;
            }
            break;
        case RX_STATE_WAIT_CRC:
            xSemaphoreTake(crcMutex, portMAX_DELAY);
            HAL_CRC_Calculate(&hcrc, (uint32_t*)(&m_header), HEADER_SIZE_WORD);
            crc = HAL_CRC_Accumulate(&hcrc, &packet_data_offset[packet_data_w-1], m_header.packet_len + HEADER_SIZE_WORD);
            xSemaphoreGive(crcMutex);
            if (crc == d) {
                // Signify destination port of pending packet
                break;
            } else {
                // Remove corrupt packet
                packet_data_offset_w--;
            }
            break;
        case RX_STATE_LISTEN:
            __tcp_parse_header(d);
            htcp.rx_state = RX_STATE_LISTEN_PACKET;
            break;
        case RX_STATE_LISTEN_PACKET:
            __tcp_parse_header(d);
            if (htcp.rx_count == HEADER_SIZE_WORD) {
                if (m_header.packet_len == 0)
                    htcp.rx_state = RX_STATE_LISTEN_CRC;
                else
                    htcp.rx_state = RX_STATE_LISTEN_DATA;
            }
            break;
        case RX_STATE_LISTEN_DATA:
            if ((++htcp.rx_count) == (m_header.packet_len + HEADER_SIZE_WORD)) {
                htcp.rx_state = RX_STATE_LISTEN_CRC;
            }
            break;
        case RX_STATE_LISTEN_CRC:
            xSemaphoreTake(crcMutex, portMAX_DELAY);
            crc_1 = HAL_CRC_Calculate(&hcrc, (uint32_t*)(&m_header), HEADER_SIZE_WORD);
            xSemaphoreGive(crcMutex);
            if (crc_1 == d) {
                // Connection success [TODO]
                htcp.state = TCP_STATE_SYN_RECEIVED;
            } else
                htcp.rx_state = RX_STATE_LISTEN;
            break;
    }
}

uint32_t __tcp_parse_header(uint32_t d)
{
    // [TODO] Determine return value
    switch (htcp.rx_count) {
        case 0:
            m_header.source_port = (d & 0xff000000) >> 24;
            m_header.dest_port = (d & 0x00ff0000) >> 16;
            m_header.sequence_number = d & 0x0000ffff;
            break;
        case 1:
            m_header.ack_number = (d & 0xffff0000) >> 16;
            m_header.packet_len = d & 0x0000ffff;
            break;
        case 2:
            m_header.window = (d & 0xffff0000) >> 16;
            m_header.flag = (d & 0x0000ff00) >> 8;
            break;
            //m_header.reserved = (d & 0x000000ff);
        case 3:
            m_header.key = d;
            break;
        default:
            // Ignore all
            break;
    }
    htcp.rx_count++;
}