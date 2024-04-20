#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"                  // Device header
#include "PWM.h"


void Servo_Init(void);
void Servo_SetAngle(float Angle);

#endif
