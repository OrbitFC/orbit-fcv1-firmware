#ifndef __TASK_SEMPHR_H__
#define __TASK_SEMPHR_H__

#include <FreeRTOS.h>
#include <semphr.h>

extern SemaphoreHandle_t spiHwSemphr;
extern SemaphoreHandle_t sensorI2cSemphr;

extern SemaphoreHandle_t crcMutex;
extern SemaphoreHandle_t rngMutex;
#endif