#include "sensor/bno055.h"


struct sensorInfo bno055SensorInfo = {
    .device_id = BNO055_SENSOR_ID,
    .name = "BNO055",
    .rate = 100,
    .sensor_type = SENSOR_TYPE_GYROSCOPE | SENSOR_TYPE_MAGNETIC | SENSOR_TYPE_ACCELERATION,
    .redundancy = 1
};

unsigned __bno055_i2c_read(uint8_t *rx, size_t size, uint16_t mem_addr)
{
    return __sensor_i2c_read(rx, size, BNO055_I2C_ADDR, mem_addr, BNO055_INSTANCE);
}

unsigned __bno055_i2c_write(uint8_t *tx, size_t size, uint16_t mem_addr)
{
    return __sensor_i2c_write(tx, size, BNO055_I2C_ADDR, mem_addr, BNO055_INSTANCE);
}

unsigned __bno055_i2c_read_single(uint8_t *rx, uint16_t mem_addr)
{
    return __bno055_i2c_read(rx, 1, mem_addr);
}

unsigned __bno055_i2c_write_single(uint8_t tx, uint16_t mem_addr)
{
    // [TODO] Copy single byte to memory location specific for DMA transfer
    return __bno055_i2c_write(&tx, 1, mem_addr);
}