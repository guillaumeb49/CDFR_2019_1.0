/*
 * A_Servo.c
 *
 *  Created on: 19 mai 2019
 *      Author: guillaume
 */

#include "A_Servo.h"

void F_Init_TimerServo()
{
	// Enable the Timer 9
	RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;

	// Init GPIO
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; // Port E : clock enable

	GPIOE->MODER &= ~(GPIO_MODER_MODER5);	// PE5: Alternate --> ServoMotor 1
	GPIOE->MODER |=  (0x02 << 10);
	GPIOE->MODER &= ~(GPIO_MODER_MODER6);	// PE6: Alternate --> ServoMotor 2
	GPIOE->MODER |=  (0x02 << 12);

	GPIOE->AFR[0] &= ~(0x0F << 20);	// AF3 on PE5--> Timer 9 channel 1 --> ServoMotor 1
	GPIOE->AFR[0] |=  (3    << 20);
	GPIOE->AFR[0] &= ~(0x0F << 24);	// AF3 on PE6--> Timer 9 channel 2 --> ServoMotor 2
	GPIOE->AFR[0] |=  (3    << 24);


	TIM9->PSC = 840-1;	// Prescaler
	TIM9->ARR = 4000-1;	// 20 ms (PWM Frequency)

	TIM9->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;	// PWM mode 1 upcounting
	TIM9->CCMR1 &= ~TIM_CCMR1_OC2M_0;

	TIM9->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;	// PWM mode 1 upcounting
	TIM9->CCMR1 &= ~TIM_CCMR1_OC1M_0;

	TIM9->CCMR1 |= TIM_CCMR1_OC2PE;	// Preload Enable
	TIM9->CCMR1 &= ~TIM_CCMR1_CC2S;	// Output
	TIM9->CCER 	|= TIM_CCER_CC2E;	// Channel is ON
	TIM9->CCMR1 |= TIM_CCMR1_OC1PE;	// Preload Enable
	TIM9->CCMR1 &= ~TIM_CCMR1_CC1S;	// Output
	TIM9->CCER 	|= TIM_CCER_CC1E;	// Channel is ON

	TIM9->CR1 	|= TIM_CR1_ARPE;	// Auto reload
	TIM9->EGR 	|= TIM_EGR_UG;		// Reinitialize the counter
	TIM9->SR 	&= TIM_SR_UIF;		// Clear update interrupt flag
	TIM9->BDTR 	|= TIM_BDTR_MOE;	// Main output Enable

	TIM9->CCR1 = 2000-1; 	// Set duty cycle

	TIM9->CCR2 = 200-1; 	// Set duty cycle

	// Start Timer 1
	TIM9->CR1 |= TIM_CR1_CEN;

}
