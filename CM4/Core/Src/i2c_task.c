#include "main.h"
#include "i2c.h"
#include "task_func.h"
#include "task_queue.h"
#include "task_semphr.h"

#define NO_I2C 4

typedef I2C_HandleTypeDef *I2C_H;

I2C_H h_i2c[] = {&hi2c1, &hi2c2, &hi2c3, &hi2c4};

void i2cTask(void *_param)
{
    i2cRequestTypeDef req = NULL;
    i2cRequestTypeDef reqt[] = {NULL, NULL, NULL, NULL};
    HAL_StatusTypeDef stat = HAL_OK;
    while (1) {
        BaseType_t ret = xQueueReceive(i2cQueue, &req, portMAX_DELAY);
        if (ret == pdPASS) {
            if (req->inst >= NO_I2C) {
                *(req->status = I2C_STATUS_BYTE0_INVALID_INSTANCE);
                xSemaphoreGive(req->semphr);
                goto BREAK;
            }

            reqt[req->inst] = req;

            switch (req->type) {
                case I2C_REQUEST_READ:
                    HAL_I2C_Mem_Read_DMA(h_i2c[req->inst], req->dev_addr, req->mem_addr, I2C_MEMADD_SIZE_8BIT, req->rx, req->size);
                    break;
                case I2C_REQUEST_WRITE:
                    HAL_I2C_Mem_Write_DMA(h_i2c[req->inst], req->dev_addr, req->mem_addr, I2C_MEMADD_SIZE_8BIT, req->tx, req->size);
                    break;
                case I2C_REQUEST_FINISH_I2C1:
                    xSemaphoreGive(reqt[0]->semphr);
                    break;
                case I2C_REQUEST_FINISH_I2C2:
                    xSemaphoreGive(reqt[1]->semphr);
                    break;
                case I2C_REQUEST_FINISH_I2C3:
                    xSemaphoreGive(reqt[2]->semphr);
                    break;
                case I2C_REQUEST_FINISH_I2C4:
                    xSemaphoreGive(reqt[3]->semphr);
                    break;
            }


BREAK:
        }
    }
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    BaseType_t q;
    static struct i2cRequest req;
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        if (hi2c == &hi2c1) {
            req.type = I2C_REQUEST_FINISH_I2C1;
            xQueueSendToFrontFromISR(i2cQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hi2c == &hi2c2) {
            req.type = I2C_REQUEST_FINISH_I2C2;
            xQueueSendToFrontFromISR(i2cQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hi2c == &hi2c3) {
            req.type = I2C_REQUEST_FINISH_I2C3;
            xQueueSendToFrontFromISR(i2cQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hi2c == &hi2c4) {
            req.type = I2C_REQUEST_FINISH_I2C4;
            xQueueSendToFrontFromISR(i2cQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        }
    }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    BaseType_t q;
    static struct i2cRequest req;
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        if (hi2c == &hi2c1) {
            req.type = I2C_REQUEST_FINISH_I2C1;
            xQueueSendToFrontFromISR(i2cQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hi2c == &hi2c2) {
            req.type = I2C_REQUEST_FINISH_I2C2;
            xQueueSendToFrontFromISR(i2cQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hi2c == &hi2c3) {
            req.type = I2C_REQUEST_FINISH_I2C3;
            xQueueSendToFrontFromISR(i2cQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        } else if (hi2c == &hi2c4) {
            req.type = I2C_REQUEST_FINISH_I2C4;
            xQueueSendToFrontFromISR(i2cQueue, &req, &q);
            portYIELD_FROM_ISR(q);
        }
    }
}