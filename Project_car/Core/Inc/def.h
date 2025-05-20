

#ifndef COMMON_DEF_H_
#define COMMON_DEF_H_

#include <stdio.h>
#include "string.h"
#include <stdint.h>
#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

typedef struct RCCar_State{
	uint8_t command;
	uint16_t d11;
	uint16_t d12;
	uint16_t d1;
	uint16_t speed;
	uint8_t obstacle_flag;
	uint8_t auto_flag;
	uint8_t rotating_flag;
	uint32_t rotate_end_tick;
}Car;

#include "delay.h"
#include "ultrasonic.h"
#include "driving.h"


#endif /* COMMON_DEF_H_ */
