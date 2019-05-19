/*
 * D_PWM.h
 *
 *  Created on: 11 juil. 2016
 *      Author: guill
 */

#ifndef D_PWM_H_
#define D_PWM_H_

#include <stm32f4xx.h>
#include "main.h"
#include "tim.h"

#define EN_NZ_CORRECTION 1
#define NZ_CORRECTION 15

void Init_Motors(void);
void F_PWM_SetCmdMotorDroit(int speed);
void F_PWM_SetCmdMotorGauche(int speed);
void F_PWM_SetDCMotorDroit(int dc);
void F_PWM_SetDCMotorGauche(int dc);
#endif /* D_PWM_H_ */
