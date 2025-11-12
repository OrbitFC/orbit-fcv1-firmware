#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>

#define QUEUE_SPI_SIZE 12
#define QUEUE_I2C_SIZE 12
#define QUEUE_UART_SIZE 12
#define QUEUE_CAN_SIZE 8

// Request type
enum SPI_REQUEST {
    SPI_REQUEST_TRANSMIT,
    SPI_REQUEST_RECEIVE,
    SPI_REQUEST_TRANSMIT_RECEIVE,
    SPI_REQUEST_FINISH_SPI1,
    SPI_REQUEST_FINISH_SPI2
};

enum UART_REQUEST {
    UART_REQUEST_TRANSMIT,
    UART_REQUEST_RECEIVE
};

enum I2C_REQUEST {
    I2C_REQUEST_READ,
    I2C_REQUEST_WRITE,
    I2C_REQUEST_FINISH_I2C1,
    I2C_REQUEST_FINISH_I2C2,
    I2C_REQUEST_FINISH_I2C3,
    I2C_REQUEST_FINISH_I2C4
};

enum CAN_REQUEST {
    CAN_REQUEST_TRANSMIT,
    CAN_REQUEST_RECEIVE
};


// Instances
enum SPI_INSTANCE {
    SPI_INSTANCE_SPI1,
    SPI_INSTANCE_SPI2
};

enum I2C_INSTANCE {
    I2C_INSTANCE_I2C1,
    I2C_INSTANCE_I2C2,
    I2C_INSTANCE_I2C3,
    I2C_INSTANCE_I2C4
};

enum UART_INSTANCE {
    UART_INSTANCE_LPUART1,
    UART_INSTANCE_UART4,
    UART_INSTANCE_UART5,
    UART_INSTANCE_UART7
};

struct spiRequest {
    enum SPI_REQUEST type;
    uint8_t *tx;
    uint8_t *rx;
    size_t size;
    enum SPI_INSTANCE inst;
    unsigned channel;
    SemaphoreHandle_t semphr;
    unsigned *status;
};

typedef struct spiRequest* spiRequestTypeDef;

struct uartRequest {
    enum UART_REQUEST type;
    uint8_t *tx;
    uint8_t *rx;
    size_t size;
    enum UART_INSTANCE inst;
    SemaphoreHandle_t semphr;
    unsigned *status;
};

typedef struct uartRequest* uartRequestTypeDef;

struct i2cRequest {
    enum I2C_REQUEST type;
    uint8_t *tx;
    uint8_t *rx;
    size_t size;
    uint16_t dev_addr;   // 17-bit address is now unsupported
    uint16_t mem_addr;
    enum I2C_INSTANCE inst;
    SemaphoreHandle_t semphr;
    unsigned status;
};

typedef struct i2cRequest* i2cRequestTypeDef;


struct canRequest {
    enum CAN_REQUEST type;
    uint8_t *tx;
    uint8_t *rx;
    size_t size;
    SemaphoreHandle_t semphr;
    unsigned *status;
};

typedef struct canRequest* canRequestTypeDef;

extern QueueHandle_t uartQueue;
extern QueueHandle_t spiQueue;
extern QueueHandle_t i2cQueue;
extern QueueHandle_t canQueue;
#endif