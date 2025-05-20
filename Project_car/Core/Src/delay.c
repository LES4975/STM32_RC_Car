#include "delay.h"

void delay_us(uint16_t us) // HAL_Delay()와 비슷한 함수다
{
	__HAL_TIM_SET_COUNTER(&htim4, 0); // Timer Counter Register Value Setting
	while((__HAL_TIM_GET_COUNTER(&htim4)) < us); //TIM Counter Register value on runtime을 갖고 와서 us와 비교
}
