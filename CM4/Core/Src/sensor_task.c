#include "sensor.h"
#include "task_func.h"
#include "task.h"
#include "task_queue.h"

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
    for (int i = 0; i < NO_SENSOR; i++) {
        if (sensorList[i].device_id != 0) {
            if (sensorList[i].sensor_scan()) {
                sensorList[i].status |= 1 << STATUS_UP;
            } else
                goto SENSOR_FAIL_0;
            
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