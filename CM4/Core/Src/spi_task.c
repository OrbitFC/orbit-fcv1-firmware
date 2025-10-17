#include "main.h"
#include "spi.h"
#include "task_func.h"
#include "task_queue.h"
#include "task_semphr.h"
#include "debug.h"

#define NO_SPI 2

typedef SPI_HandleTypeDef* SPI_H;

SPI_H h_spi[] = {&hspi1, &hspi2};
uint16_t no_channel_hspi[] = {4, 2};
GPIO_TypeDef port[4][2] = {{SPI1_NSS1_GPIO_Port, SPI1_NSS2_GPIO_Port, SPI1_NSS3_GPIO_Port, SPI1_NSS4_GPIO_Port},
                            {SPI2_NSS1_GPIO_Port, SPI2_NSS2_GPIO_Port, NULL, NULL}};
uint16_t pin[4][2] = {{SPI1_NSS1_Pin, SPI1_NSS2_Pin, SPI1_NSS3_Pin, SPI1_NSS4_Pin},
                        {SPI2_NSS1_Pin, SPI2_NSS2_Pin, 0xFF, 0xFF}};

void spiTask(void *_param)
{
    spiRequestTypeDef req = NULL, req_spi1 = NULL, req_spi2 = NULL;
    HAL_StatusTypeDef stat = HAL_OK;
    while (1) {
        BaseType_t ret = xQueueReceive(spiQueue, &req, portMAX_DELAY);
        if (ret == pdPASS) {
            if (req->inst >= NO_SPI) {
                    *(req->status) |= STATUS_INVALID_INSTANCE;
                    xSemaphoreGive(req->semphr);
                    goto BREAK;
                }
                    
            if (req->channel > no_channel_hspi[req->inst]) {
                *(req->status) |= STATUS_INVALID_CHANNEL;
                xSemaphoreGive(req->semphr);
                goto BREAK;
            }

            if (req->inst == SPI_INSTANCE_SPI1)
                req_spi1 = req;
            else if (req->inst == SPI_INSTANCE_SPI2)
                req_spi2 = req;

            HAL_GPIO_WritePin(port[req->inst][req->channel], pin[req->inst][req->channel], GPIO_PIN_RESET);

            switch (req->type) {
                case SPI_REQUEST_FINISH_SPI1:
                    HAL_GPIO_WritePin(port[req_spi1->inst][req_spi1->channel], pin[req_spi1->inst][req_spi1->channel], GPIO_PIN_SET);
                    *(req->status) = STATUS_OK;
                    xSemaphoreGive(req_spi1->semphr);
                    break;
                case SPI_REQUEST_FINISH_SPI2:
                    HAL_GPIO_WritePin(port[req_spi2->inst][req_spi2->channel], pin[req_spi2->inst][req_spi2->channel], GPIO_PIN_SET);
                    *(req->status) = STATUS_OK;
                    xSemaphoreGive(req_spi2->semphr);
                    break;
                case SPI_REQUEST_TRANSMIT:
                    stat = HAL_SPI_Transmit_DMA(h_spi[req->inst], req->tx, req->size);
                    break;
                case SPI_REQUEST_RECEIVE:
                    stat = HAL_SPI_Receive_DMA(h_spi[req->inst], req->rx, req->size);
                    break;
                case SPI_REQUEST_TRANSMIT_RECEIVE:
                    stat = HAL_SPI_TransmitReceive_DMA(h_spi[req->inst], req->tx, req->rx, req->size);
                    break;
            }
BREAK:
        }
    }
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    BaseType_t q;
    static struct spiRequest req;
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        if (hspi == &hspi1) {
            req.type = SPI_REQUEST_FINISH_SPI1;
            xQueueSendToFrontFromISR(spiQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hspi == &hspi2) {
            req.type = SPI_REQUEST_FINISH_SPI2;
            xQueueSendToFrontFromISR(spiQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        }
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    BaseType_t q;
    static struct spiRequest req;
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        if (hspi == &hspi1) {
            req.type = SPI_REQUEST_FINISH_SPI1;
            xQueueSendToFrontFromISR(spiQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hspi == &hspi2) {
            req.type = SPI_REQUEST_FINISH_SPI2;
            xQueueSendToFrontFromISR(spiQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        }
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    BaseType_t q;
    static struct spiRequest req;
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        if (hspi == &hspi1) {
            req.type = SPI_REQUEST_FINISH_SPI1;
            xQueueSendToFrontFromISR(spiQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hspi == &hspi2) {
            req.type = SPI_REQUEST_FINISH_SPI2;
            xQueueSendToFrontFromISR(spiQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        }
    }
}

