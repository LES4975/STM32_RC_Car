
#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "def.h"

#define UTS_PORT		GPIOC
#define	UTS_PIN			GPIO_PIN_10

void HCSR04_Trigger();

void searchDirection(Car *car);
uint32_t getRotationDuration(uint16_t target_dist, uint16_t min_safe_dist);

#endif /* INC_ULTRASONIC_H_ */
