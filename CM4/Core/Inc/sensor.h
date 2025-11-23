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
    SENSOR_TYPE_GNSS = 0x80,
};

enum SENSOR_DATA {
    SENSOR_DATA_ACC_X,
    SENSOR_DATA_ACC_Y,
    SENSOR_DATA_ACC_Z,
    SENSOR_DATA_GYRO_X,
    SENSOR_DATA_GYRO_Y,
    SENSOR_DATA_GYRO_Z,
    SENSOR_DATA_MAG_X,
    SENSOR_DATA_MAG_Y,
    SENSOR_DATA_MAG_Z,
    SENSOR_DATA_P,
    SENSOR_DATA_T,
    SENSOR_DATA_DIFF_P,
    SENSOR_DATA_OPTF,
    SENSOR_DATA_GNSS_LAT,
    SENSOR_DATA_GNSS_LON,
    SENSOR_DATA_GNSS_ALT
};

/**
 * Derived data require an origin  (for position), a (static to Earth) coordination frame. This is set during calibration phase (on the ground) and will not change during flight.
 */
enum SENSOR_DDATA { // Derived data
    SENSOR_DDATA_EULER_ALPHA,
    SENSOR_DDATA_EULER_BETA,
    SENSOR_DDATA_EULER_GAMMA,
    SENSOR_DDATA_QUAT_A,        // Quaternion is stored as a + bi + cj + dk
    SENSOR_DDATA_QUAT_B,
    SENSOR_DDATA_QUAT_C,
    SENSOR_DDATA_QUAT_D,
    SENSOR_DDATA_ALT,
    SENSOR_DDATA_GROUND_V,
    SENSOR_DDATA_AIR_V,
    SENSOR_DDATA_X,
    SENSOR_DDATA_Y,
    SENSOR_DDATA_Z
};

enum SENSOR_ID {
    SENSOR_ID_NONE,
    SENSOR_ID_BNO055,
    SENSOR_ID_BMI088,
    SENSOR_ID_ICM42688,
    SENSOR_ID_BMP585,
    SENSOR_ID_MTF01,
    SENSOR_ID_GNSS,
};

#define NO_SENSOR 8

#define STATUS_UP 0
#define STATUS_CALIBRATED 1
#define STATUS_ONBOARD 2
#define STATUS_ACTIVE 3

#define MANDATE_SENSOR_TYPE (SENSOR_TYPE_GYROSCOPE | SENSOR_TYPE_MAGNETIC SENSOR_TYPE_PRESSURE | SENSOR_TYPE_ACCELERATION)
struct sensorInfo {
    uint32_t device_id; // Ignore those with device id = 0
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
    // Byte 2: Set for on-board, reset for not on-board
    // Byte 3: Set for active (used for data), reset for inactive (maybe available but used as a fallback)
    uint8_t status;
};

// M4 core initialization must initialize sensorList and set all null struct sensorInfo's device ID to 0
extern struct sensorInfo sensorList[NO_SENSOR];

#define QUEUE_SENSOR_SIZE 15

enum SENSOR_REQUEST_TYPE {
    SENSOR_REQUEST_TYPE_CALIB = 0xF0,
    SENSOR_REQUEST_TYPE_START = 0xFF,
    SENSOR_REQUEST_TYPE_STOP = 0x00,
    SENSOR_BEGIN_SEND_DATA = 0x02,
    SENSOR_BEGIN_STOP_DATA = 0x01,
    SENSOR_REQUEST_TYPE_CALIB_MOUNTING = 0xF1,  // Calibrate for mounting position. This return relative pitch and roll 
    SENSOR_REQUEST_TYPE_CALIB_ACC = 0xF2,
    SENSOR_REQUEST_TYPE_CALIB_GYRO = 0xF3,
    SENSOR_REQUEST_TYPE_CALIB_MAG = 0xF4

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