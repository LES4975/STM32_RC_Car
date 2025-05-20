
#include "ultrasonic.h"
#include "delay.h"
#include "cmsis_os2.h"

static volatile uint16_t IC_Value1[3] = {0}; // Echo Pin이 Rising Edge일 때 갖고 오는 것
static volatile uint16_t IC_Value2[3] = {0}; // Echo Pin이 Falling Edge일 때 갖고 오는 것
static uint16_t echoTime[3] = {0}; // Echo Pin의 Rising과 Falling Edge 사이의 시간(count 갯수)
static uint8_t captureFlag[3] = {0}; // capture가 되면 세우는 플래그
uint16_t distance[3] = {0}; // 거리


void HCSR04_Trigger()
{

	HAL_GPIO_WritePin(UTS_PORT, UTS_PIN, 0); // 혹시 모르니까 초기 상태 확실히 정해 주기
	delay_us(2);

	HAL_GPIO_WritePin(UTS_PORT, UTS_PIN, 1);
	delay_us(10); // High 상태를 10us만큼 유지
	HAL_GPIO_WritePin(UTS_PORT, UTS_PIN, 0);


	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1); // 인터럽트를 Enable, TIM1의 CH1 인터럽트 활성화
	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC1); // 인터럽트를 Enable, TIM2의 CH1 인터럽트 활성화
	__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1); // 인터럽트를 Enable, TIM4의 CH1 인터럽트 활성화

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) // 캡처하면 Callback
{

	    if(htim->Instance == htim1.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	    {
	    	if(captureFlag[0] == 0)
	    		        {
	    		            IC_Value1[0] = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
	    		            captureFlag[0] = 1;
	    		            __HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
	    		        }
	    		        else
	    		        {
	    		            IC_Value2[0] = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);

	    		            if(IC_Value2[0] >= IC_Value1[0])
	    		                echoTime[0] = IC_Value2[0] - IC_Value1[0];
	    		            else
	    		                echoTime[0] = (0xFFFF - IC_Value1[0]) + IC_Value2[0];

	    		            distance[0] = echoTime[0] / 58;
	    		            captureFlag[0] = 0;

	    		            __HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
	    		            __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
	    		        }
	    }

	    if(htim->Instance == htim2.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	    {
	    	if(captureFlag[1] == 0)
	    		        {
	    		            IC_Value1[1] = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
	    		            captureFlag[1] = 1;
	    		            __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
	    		        }
	    		        else
	    		        {
	    		            IC_Value2[1] = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);

	    		            if(IC_Value2[1] >= IC_Value1[1])
	    		                echoTime[1] = IC_Value2[1] - IC_Value1[1];
	    		            else
	    		                echoTime[1] = (0xFFFF - IC_Value1[1]) + IC_Value2[1];

	    		            distance[1] = echoTime[1] / 58;
	    		            captureFlag[1] = 0;

	    		            __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
	    		            __HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC1);
	    		        }
	    }

	    if(htim->Instance == htim4.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	    	    {
	    	    	if(captureFlag[2] == 0)
	    	    		        {
	    	    		            IC_Value1[2] = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);
	    	    		            captureFlag[2] = 1;
	    	    		            __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
	    	    		        }
	    	    		        else
	    	    		        {
	    	    		            IC_Value2[2] = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);

	    	    		            if(IC_Value2[2] >= IC_Value1[2])
	    	    		                echoTime[2] = IC_Value2[2] - IC_Value1[2];
	    	    		            else
	    	    		                echoTime[2] = (0xFFFF - IC_Value1[2]) + IC_Value2[2];

	    	    		            distance[2] = echoTime[2] / 58;
	    	    		            captureFlag[2] = 0;

	    	    		            __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
	    	    		            __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC1);
	    	    		        }
	    	    }
}

void searchDirection(Car *car)
{
	uint32_t now = osKernelGetTickCount();
	if(car->rotating_flag)
	{
		if(now >= car->rotate_end_tick)
		{
			car->rotating_flag = 0;

			if(car->d12 > 20)
				car->obstacle_flag = 0;
			else
				car->obstacle_flag = 1;
		}
	}
	else
	{
		if(car->d12 < 20 || car->d11 < 20 || car->d1 < 20)
		{
			if(car->d11 > car->d1 && car->d11 > 20)
			{
				drivingControl('L');
				car->rotating_flag = 1;
				car->rotate_end_tick = now + getRotationDuration(car->d11, 20);
			}
			else if(car->d1 > 20)
			{
				drivingControl('R');
				car->rotating_flag = 1;
				car->rotate_end_tick = now + getRotationDuration(car->d1, 20);
			}
		}
		else if(car->d12 > 30 && car->d11 > 25 && car->d1 > 25)
			car->obstacle_flag = 0;
	}
//	if(car->d12 < 20)
//	{
//		drivingControl('S');
//		if(car->d11 > car->d1 && car->d11 > 20)
//			drivingControl('L');
//		else if(car->d1 > 20)
//			drivingControl('R');
//		else
//			drivingControl('B');
//	}
//	else if (car->d12 > 30)
//	{
//		car->obstacle_flag = 0;
//	}
}

uint32_t getRotationDuration(uint16_t target_dist, uint16_t min_safe_dist)
{
	// 목표 거리까지 몇 cm 부족한가?
	if (target_dist >= min_safe_dist)
		return 200; // 최소 회전 시간

	uint16_t deficit = min_safe_dist - target_dist;

	// 1cm 부족할 때마다 20ms 더 돌림 (튜닝 필요)
	return 200 + (deficit * 20); // 예: 5cm 부족 → 300 + 100 = 400ms
}

