/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "task_func.h"
#include "task_queue.h"
#include "sensor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Stack depth definition
#define TASK_UART_STACK_DEPTH 128
#define TASK_I2C_STACK_DEPTH 128
#define TASK_SPI_STACK_DEPTH 128
#define TASK_CAN_STACK_DEPTH 128

// Priority definition
#define TASK_UART_PRIORITY 10
#define TASK_SPI_PRIORITY 11
#define TASK_CAN_PRIORITY 9
#define TASK_I2C_PRIORITY 12
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

TaskHandle_t uartTaskHandle;
TaskHandle_t spiTaskHandle;
TaskHandle_t canTaskHandle;
TaskHandle_t i2cTaskHandle;
struct TaskAttr {
    TaskFunction_t task;
    const char * const name;
    configSTACK_DEPTH_TYPE stack_depth;
    void *param;
    UBaseType_t priority;
    TaskHandle_t handle;
};

#define TASK_UART_POS 0
#define TASK_I2C_POS 1
#define TASK_SPI_POS 2
#define TASK_CAN_POS 3

struct TaskAttr taskList[] = {
    {
        .task = uartTask,
        .name = "UART",
        .stack_depth = TASK_UART_STACK_DEPTH * 4,
        .param = NULL,
        .priority = TASK_UART_PRIORITY
    }, {
        .task = i2cTask,
        .name = "I2C",
        .stack_depth = TASK_I2C_STACK_DEPTH * 4,
        .param = NULL,
        .priority = TASK_I2C_PRIORITY
    }, {
        .task = spiTask,
        .name = "SPI",
        .stack_depth = TASK_SPI_STACK_DEPTH * 4,
        .param = NULL,
        .priority = TASK_SPI_PRIORITY
    }, {
        .task = canTask,
        .name = "CAN",
        .stack_depth = TASK_CAN_STACK_DEPTH  * 4,
        .param = NULL,
        .priority = TASK_CAN_PRIORITY
    }
};

QueueHandle_t uartQueue;
QueueHandle_t spiQueue;
QueueHandle_t i2cQueue;
QueueHandle_t canQueue;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    BaseType_t _t;
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    uartQueue = xQueueCreate(QUEUE_UART_SIZE, sizeof(uartRequestTypeDef));
    if (uartQueue == NULL)
        Error_Handler();
    spiQueue = xQueueCreate(QUEUE_SPI_SIZE, sizeof(spiRequestTypeDef));
    if (spiQueue == NULL)
        Error_Handler();
    i2cQueue = xQueueCreate(QUEUE_I2C_SIZE, sizeof(i2cRequestTypeDef));
    if (i2cQueue == NULL)
        Error_Handler();
    canQueue = xQueueCreate(QUEUE_CAN_SIZE, sizeof(canRequestTypeDef));
    if (canQueue == NULL)
        Error_Handler();
    sensorQueue = xQueueCreate(QUEUE_SENSOR_SIZE, sizeof(sensorRequestTypeDef));
    if (sensorQueue == NULL)
        Error_Handler();

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    for (int i = 0; i < (sizeof(taskList) / sizeof(taskList[0])); i++) {
        _t = xTaskCreate(taskList[i].task, taskList[i].name, taskList[i].stack_depth, taskList[i].param, taskList[i].priority, &(taskList[i].handle));
        if (_t != pdPASS)
            Error_Handler();
    }

    uartTaskHandle = taskList[TASK_UART_POS].handle;
    spiTaskHandle = taskList[TASK_SPI_POS].handle;
    canTaskHandle = taskList[TASK_CAN_POS].handle;
    i2cTaskHandle = taskList[TASK_I2C_POS].handle;
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

