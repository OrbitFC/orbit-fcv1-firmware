#include "sensor.h"

struct sensorInfo sensorList[NO_SENSOR] = {
    {
        .device_id = SENSOR_ID_BNO055,
        .rate = 100,
        .sensor_type = SENSOR_TYPE_GYROSCOPE | SENSOR_TYPE_MAGNETIC | SENSOR_TYPE_ACCELERATION,
        .redundancy = 1,
        .status = 1 << STATUS_ONBOARD
    }, {
        .device_id = SENSOR_ID_BMI088,
        .rate = 100,
        .sensor_type = SENSOR_TYPE_GYROSCOPE | SENSOR_TYPE_ACCELERATION,
        .redundancy = 1,
        .status = 0
    }, {
        .device_id = SENSOR_ID_ICM42688,
        .rate = 100,
        .sensor_type = SENSOR_TYPE_GYROSCOPE | SENSOR_TYPE_ACCELERATION,
        .redundancy = 2,
        .status = 1 << STATUS_ONBOARD
    }, {
        .device_id = SENSOR_ID_BMP585,
        .rate = 1,
        .sensor_type = SENSOR_TYPE_PRESSURE,
        .redundancy = 1,
        .status = 1 << STATUS_ONBOARD
    }, {
        .device_id = SENSOR_ID_MTF01,
        .rate = 1,
        .sensor_type = SENSOR_TYPE_RANGEFINDER | SENSOR_TYPE_OPTICAL_FLOW,
        .redundancy = 1,
        .status = 0
    }, {
        .device_id = SENSOR_ID_GNSS,
        .rate = 1,
        .sensor_type = SENSOR_TYPE_GNSS,
        .redundancy = 1,
        .status = 0
    }
};
