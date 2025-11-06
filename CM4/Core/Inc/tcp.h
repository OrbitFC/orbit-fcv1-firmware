#include <stdint.h>
#include <stddef.h>
#include <stdalign.h>

#define FIN 0
#define SYN 1
#define RST 2
#define PSH 3
#define ACK 4
#define URG 5
#define ECE 6
#define CWR 7

#define MAX_WINDOW 256
#define PACKET_BUFFER_SIZE 2048 // Measured in 32-bit words
#define MAX_PACKET 16

struct tcp_header {
    uint8_t source_port;
    uint8_t dest_port;
    uint16_t sequence_number;
    uint16_t ack_number;
    uint16_t packet_len;
    uint16_t window;
    uint8_t flag;
    uint8_t reserved;
    uint32_t key;
} __attribute__((packed));

#define HEADER_SIZE (sizeof(struct tcp_header))
#define HEADER_SIZE_WORD ((sizeof(struct tcp_header)) / 4)
#define EMPTY_PACKET_SIZE (sizeof(struct tcp_header) + 4)

// Packet data buffer. Each packet data's offset is stored in packet_data_offset
// packet_data[] should be stored in DMA compatible memory [TODO]
extern size_t packet_data_w, packet_data_r; // Should be initialized to 0
extern uint32_t packet_data[PACKET_BUFFER_SIZE];
extern size_t packet_data_offset[MAX_PACKET];
extern struct tcp_header tcp_header_data[MAX_PACKET];
// Control read/write over tcp_header_data and packet_data_offset
extern size_t packet_data_offset_w, packet_data_offset_r;

alignas(4) uint8_t empty_packet[EMPTY_PACKET_SIZE];
struct tcp_header *empty_packet_header = (struct tcp_header*)empty_packet;
uint32_t *empty_packet_crc = &(((uint32_t*)empty_packet)[4]);
// Implemented according to RFC793
enum TCP_STATE {
    TCP_STATE_LISTEN,
    TCP_STATE_SYN_SENT,
    TCP_STATE_SYN_RECEIVED,
    TCP_STATE_ESTABLISHED,
    TCP_STATE_FIN_WAIT_1,
    TCP_STATE_FIN_WAIT_2,
    TCP_STATE_CLOSE_WAIT,
    TCP_STATE_CLOSING,
    TCP_STATE_LAST_ACK,
    TCP_STATE_TIME_WAIT,
    TCP_STATE_CLOSED
};

enum RX_STATE {
    RX_STATE_CLOSED,
    RX_STATE_LISTEN,
    RX_STATE_ESTABLISHED,
    RX_STATE_WAIT_PACKET,
    RX_STATE_WAIT_CRC,
    RX_STATE_WAIT_DATA,
    RX_STATE_LISTEN_PACKET, // Connection unestablished
    RX_STATE_LISTEN_CRC,    // Connection unestablished
    RX_STATE_LISTEN_DATA    // Listen for data on handshake packet. This is empty for now and will be ignored.
};

struct tcpTypeDef {
    enum TCP_STATE state;
    enum RX_STATE rx_state;
    uint8_t rx_count;
    uint16_t sequence_number;
    uint16_t ack_number;
};

extern struct tcpTypeDef htcp;
extern struct tcp_header m_header;
#define TCP_OK 0
#define TCP_WARNING_UNCHANGED 1 // Warning for uncorrect usage, no change made
#define TCP_FAILED 2
#define TCP_TIMEOUT 3
#define TCP_INCORRECT

// TCP are sent and receive as word, packet has to be word-aligned

/**
 * TCP Radio usage:
 * TCP Radio utilize static and global variable, so multi-thread usage is banned. Only one TCP Radio instance will be used
 * 
 * tcpOpen() to begin listen on TCP conversation
 * tcpConnect() to initiate TCP handshake
 * tcpRxHandler() to be called on Radio mux on reading of packet from FIFO. It has power to change TCP State
 * __tcp_transmit() transmit packet according to radio config
 * __tcp_receive_packet() receive the next incoming packet in full
 */
uint32_t tcpOpen(void);
uint32_t tcpConnect(uint8_t src, uint8_t dest, uint32_t key);
uint32_t __tcp_transmit(uint8_t* packet, size_t packet_len);
uint32_t __tcp_receive_packet(struct tcp_header **, uint32_t *crc);
uint32_t __tcp_parse_header(uint32_t d);
uint32_t tcpRxHandler(uint32_t);