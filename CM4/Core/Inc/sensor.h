#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <stdint.h>


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

struct sensorInfo {
    const char * const name;
    uint8_t sensor_type;
    uint8_t redundancy;
};

void __sensorScan();
void __sensorConfig();

#endif