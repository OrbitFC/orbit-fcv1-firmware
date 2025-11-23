 #include "sensor.h"
#include "task_func.h"
#include "task.h"
#include "task_queue.h"
#include "task_semphr.h"

struct sensorInfo sensorList[NO_SENSOR];
QueueHandle_t sensorQueue;

uint32_t calibrateAll()
{
    for (int i = 0; i < NO_SENSOR; i++) {
        if (sensorList[i].device_id != 0) {
            sensorList[i].sensor_scan();
        }
    }
    // [TODO] Mixed sensor calibration
}
void sensorTask(void *_param)
{
    uint32_t _t;
    uint8_t finished_sensor_type = 0;
    for (int i = 0; i < NO_SENSOR; i++) {
        if (sensorList[i].device_id != 0) {
            if (sensorList[i].sensor_scan()) {
                sensorList[i].status |= 1 << STATUS_UP;
                if (sensorList[i].sensor_type & (~(finished_sensor_type))) {
                    // If this sensor fullfil an un-fullfilled type of sensor
                    finished_sensor_type |= sensorList[i].sensor_type;
                    sensorList[i].status |= 1 << STATUS_ACTIVE;
                } else {
                    sensorList[i].status &= ~(1 << STATUS_ACTIVE);
                }
            } else
                goto SENSOR_FAIL_0;
            if (sensorList[i].status & (1 << STATUS_ACTIVE))
                _t = sensorList[i].sensor_config();
            if (!_t)
                goto SENSOR_FAIL_0;
        
            continue;

SENSOR_FAIL_0:
            sensorList[i].status &= ~(1 << STATUS_UP);
        }
    }

    sensorRequestTypeDef req = NULL;

    while (1) {
        BaseType_t ret = xQueueReceive(sensorQueue, &req, portMAX_DELAY);
        if (ret != pdPASS)
            continue;
        if (req->type == SENSOR_REQUEST_TYPE_CALIB) {
            req->status = calibrateAll();
        }
    }

}

unsigned __sensor_i2c_read(uint8_t *rx, size_t size, uint16_t dev_addr, uint16_t mem_addr, enum I2C_INSTANCE inst)
{
    struct i2cRequest req;
    req.type = I2C_REQUEST_READ;
    req.rx = rx;
    req.size = size;
    req.dev_addr = dev_addr;
    req.mem_addr = mem_addr;
    req.inst = inst;
    req.semphr = sensorI2cSemphr;
    xQueueSendToBack(i2cQueue, &req, portMAX_DELAY);
    xSemaphoreTake(sensorI2cSemphr, portMAX_DELAY);
    return req.status;
}

unsigned __sensor_i2c_write(uint8_t *tx, size_t size, uint16_t dev_addr, uint16_t mem_addr, enum I2C_INSTANCE inst)
{
    struct i2cRequest req;
    req.type = I2C_REQUEST_WRITE;
    req.rx = tx;
    req.size = size;
    req.dev_addr = dev_addr;
    req.mem_addr = mem_addr;
    req.inst = inst;
    req.semphr = sensorI2cSemphr;
    xQueueSendToBack(i2cQueue, &req, portMAX_DELAY);
    xSemaphoreTake(sensorI2cSemphr, portMAX_DELAY);
    
    return req.status;
}