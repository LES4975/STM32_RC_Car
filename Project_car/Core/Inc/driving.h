
#ifndef INC_DRIVING_H_
#define INC_DRIVING_H_

#include "def.h"

#define WHEEL_PORT	GPIOB
#define WHEEL_L1	GPIO_PIN_14 //OUT3
#define WHEEL_L2	GPIO_PIN_13 //OUT4
#define WHEEL_R1	GPIO_PIN_1  //OUT1
#define WHEEL_R2	GPIO_PIN_15 //OUT2

void setSpeed(Car *car, uint16_t ccr);

void autoDriving(Car *car, uint8_t rx);
void manualDriving(Car *car, uint8_t command);

void drivingControl(uint8_t command);

void drivingForward(); // F
void drivingBackward(); // B
void drivingForwardLeft(); // G
void drivingForwardRight(); // H
void drivingBackwardLeft(); // I
void drivingBackwardRight(); // J
void stoppingCar(); // S
void rotateClockwise(); // L
void rotateCounterClockwise(); // R

#endif /* INC_DRIVING_H_ */
