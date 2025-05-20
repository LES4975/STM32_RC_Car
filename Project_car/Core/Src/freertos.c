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
#include "def.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern uint8_t rx_bt[1];
extern Car BoongX2;
extern uint16_t distance[3];

//uint8_t rx_distance[100];

/* USER CODE END Variables */
/* Definitions for driving_RCCar */
osThreadId_t driving_RCCarHandle;
const osThreadAttr_t driving_RCCar_attributes = {
  .name = "driving_RCCar",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for measure_distanc */
osThreadId_t measure_distancHandle;
const osThreadAttr_t measure_distanc_attributes = {
  .name = "measure_distanc",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for communication */
osThreadId_t communicationHandle;
const osThreadAttr_t communication_attributes = {
  .name = "communication",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void drivingRCCar(void *argument);
void measureDistance(void *argument);
void connectBT(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

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
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of driving_RCCar */
  driving_RCCarHandle = osThreadNew(drivingRCCar, NULL, &driving_RCCar_attributes);

  /* creation of measure_distanc */
  measure_distancHandle = osThreadNew(measureDistance, NULL, &measure_distanc_attributes);

  /* creation of communication */
  communicationHandle = osThreadNew(connectBT, NULL, &communication_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_drivingRCCar */
/**
  * @brief  Function implementing the driving_RCCar thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_drivingRCCar */
void drivingRCCar(void *argument)
{
  /* USER CODE BEGIN drivingRCCar */
  /* Infinite loop */
  for(;;)
  {
	  setSpeed(&BoongX2, BoongX2.speed);
	  if(rx_bt[0] >= '0' && rx_bt[0] <= '9')
	  {
		 BoongX2.speed = (rx_bt[0] - '0') * 100;
		 if(!BoongX2.auto_flag)
			 manualDriving(&BoongX2, BoongX2.command);
	  }

	  if(BoongX2.auto_flag)
		  autoDriving(&BoongX2, rx_bt[0]);
	  else
		  manualDriving(&BoongX2, rx_bt[0]);

	  if(rx_bt[0] == 'X')
	  {
		  BoongX2.auto_flag = 1;
		  osDelay(1);
		  continue;
	  }


	  osDelay(1);
  }
  /* USER CODE END drivingRCCar */
}

/* USER CODE BEGIN Header_measureDistance */
/**
* @brief Function implementing the measure_distanc thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_measureDistance */
void measureDistance(void *argument)
{
  /* USER CODE BEGIN measureDistance */
  /* Infinite loop */
  for(;;)
  {
	  HCSR04_Trigger();
	  BoongX2.d1 = distance[0];
	  BoongX2.d12 = distance[1];
	  BoongX2.d11 = distance[2];

	  if(BoongX2.auto_flag)
	  {
		  if(BoongX2.d11 < 15 || BoongX2.d12 < 20 || BoongX2.d1 < 15)
			  BoongX2.obstacle_flag = 1;
	//	  else
	//		  BoongX2.obstacle_flag = 0;
	  }

	  osDelay(10);
  }
  /* USER CODE END measureDistance */
}

/* USER CODE BEGIN Header_connectBT */
/**
* @brief Function implementing the communication thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_connectBT */
void connectBT(void *argument)
{
  /* USER CODE BEGIN connectBT */
  /* Infinite loop */
  for(;;)
  {
	  HAL_UART_Receive_IT(&huart6, rx_bt, sizeof(rx_bt));
	  osDelay(1);
  }
  /* USER CODE END connectBT */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

