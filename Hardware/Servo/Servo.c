#include "Servo.h"

void Servo_Init(void)
{
	PWM_Init();
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare3(Angle / 180 * 2000 + 500);
}
