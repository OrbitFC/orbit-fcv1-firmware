#ifndef __TASK_FUNC_H__
#define __TASK_FUNC_H__
#include <FreeRTOS.h>
#include <task.h>

// Low level task
void uartTask(void*);
void spiTask(void*);
void canTask(void*);
void i2cTask(void*);

extern TaskHandle_t uartTaskHandle;
extern TaskHandle_t spiTaskHandle;
extern TaskHandle_t canTaskHandle;
extern TaskHandle_t i2cTaskHandle;

#endif