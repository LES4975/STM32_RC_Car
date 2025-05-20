
#include "driving.h"
#include "tim.h"

// 자동차의 왼쪽 바퀴는 IN3, IN4와 연결
// 자동차의 오른쪽 바퀴는 IN1, IN2와 연결
// 전진: IN1과 IN3을 HIGH, IN2와 IN4를 LOW
// 후진: IN1과 IN3을 LOW, IN2와 IN4를 HIGH
// 좌-정방향: IN3을 HIGH, IN4를 LOW, 나머지는 HIGH -> 정방향 우회전
// 좌-역방향: IN3을 LOW, IN4를 HIGH, 나머지는 HIGH -> 역방향 좌회전
// 우-정방향: IN1을 HIGH, IN2를 LOW, 나머지는 HIGH -> 정방향 좌회전
// 우-역방향: IN1을 LOW, 나머지는 HIGH -> 역방향 우회전
// 좌-정&우-역: HLLH -> 시계방향 회전
// 좌-역&우-정: LHHL -> 반시계방향 회전
// 멈추려면 전부 HIGH

extern uint8_t *rx_bt;

void setSpeed(Car *car, uint16_t ccr)
{
	car->speed = ccr;
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, car->speed);
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, car->speed);
}

void autoDriving(Car *car, uint8_t rx)
{
	printf("%c\r\n", rx);
	if((rx >= 'A' && rx <= 'Z' && rx != 'X') || rx == 'x')
	{
		car->auto_flag = 0;
		return;
	}
	if(!car->obstacle_flag)
		drivingControl('F');
	else
		searchDirection(car);
}

void manualDriving(Car *car, uint8_t command)
{
	if(command)
	{
		car->command = command; // 디버깅 용으로 수신된 값을 tx에 저장
		printf("%c\r\n", car->command);
		drivingControl(car->command);
	}
}

void drivingControl(uint8_t command)
{
	switch(command)
	{
	case 'F':
		drivingForward();
		break;
	case 'B':
		drivingBackward();
		break;
	case 'L':
		rotateCounterClockwise();
		break;
	case 'R':
		rotateClockwise();
		break;
	case 'G':
		drivingForwardLeft();
		break;
	case 'H':
		drivingForwardRight();
		break;
	case 'I':
		drivingBackwardLeft();
		break;
	case 'J':
		drivingBackwardRight();
		break;
	case 'S':
		stoppingCar();
		break;
	default:
		stoppingCar();
		break;

	}
}

void drivingForward()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_RESET);
}

void drivingBackward()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_SET);
}

void drivingForwardLeft()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_RESET);
}

void drivingForwardRight()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_SET);
}

void drivingBackwardLeft()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_SET);
}

void drivingBackwardRight()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_SET);
}

void stoppingCar()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_SET);
}

void rotateClockwise()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_SET);
}

void rotateCounterClockwise()
{
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_L2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WHEEL_PORT, WHEEL_R2, GPIO_PIN_RESET);
}

