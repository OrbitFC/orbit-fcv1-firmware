#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "task_queue.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include <queue.h>

#define STATUS_ENABLE 0x01

#define SENSOR_NAME_MAX_LEN 16

enum SENSOR_TYPE {
    SENSOR_TYPE_GYROSCOPE = 0x01,
    SENSOR_TYPE_MAGNETIC = 0x02,
    SENSOR_TYPE_ACCELERATION = 0x04,
    SENSOR_TYPE_PRESSURE = 0x08,
    SENSOR_TYPE_AIRSPEED = 0x10,
    SENSOR_TYPE_RANGEFINDER = 0x20,
    SENSOR_TYPE_OPTICAL_FLOW = 0x40,
    SENSOR_TYPE_GNSS = 0x80
};

#define NO_SENSOR 8

#define STATUS_UP 0
#define STATUS_CALIBRATED 1

struct sensorInfo {
    uint32_t device_id; // Ignore those with device id = 0
    char name[SENSOR_NAME_MAX_LEN];
    uint32_t rate;
    uint32_t (*sensor_scan)(void);
    uint32_t (*sensor_config)(void);
    uint32_t (*sensor_calibration)(void);
    uint32_t (*sensor_start_reading)(void);
    uint32_t (*sensor_stop_reading)(void);
    uint8_t* (*sensor_data)(void);

    uint8_t sensor_type;
    uint8_t redundancy;
    
    // Status byte format
    // Byte 0: Set for up, reset for down
    // Byte 1: Set for calibrated, reset for un-calibrated
    uint8_t status;
};

// M4 core initialization must initialize sensorList and set all null struct sensorInfo's device ID to 0
extern struct sensorInfo sensorList[NO_SENSOR];

#define QUEUE_SENSOR_SIZE 15

enum SENSOR_REQUEST_TYPE {
    SENSOR_REQUEST_TYPE_CALIB,
    SENSOR_REQUEST_TYPE_START,
    SENSOR_REQUEST_TYPE_STOP
};
struct sensorRequest {
    enum SENSOR_REQUEST_TYPE type;
    uint32_t status;
};

extern QueueHandle_t sensorQueue;

typedef struct sensorRequest* sensorRequestTypeDef;

// Generic sensor I2C read/write. All sensor must call these functions
unsigned __sensor_i2c_read(uint8_t *rx, size_t size, uint16_t dev_addr, uint16_t mem_addr, enum I2C_INSTANCE inst);
unsigned __sensor_i2c_write(uint8_t *tx, size_t size, uint16_t dev_addr, uint16_t mem_addr, enum I2C_INSTANCE inst);
#endif