/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim13;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM3 init function */
void MX_TIM3_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM4 init function */
void MX_TIM4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

	/**TIM4 GPIO Configuration
	PD14     ------> TIM4_CH3
	PD15     ------> TIM4_CH4
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	// Init Timer 4 CH4
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;	// Enable clock on timer 4
	TIM4->PSC = 1;	// Prescaler

	TIM4->ARR = 2250-1;	// 20 KHz (PWM Frequency)

	TIM4->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;	// PWM mode 1 upcounting
	TIM4->CCMR2 &= ~TIM_CCMR2_OC4M_0;

	TIM4->CCMR2 |= TIM_CCMR2_OC4PE;	// Preload Enable
	TIM4->CCMR2 &= ~TIM_CCMR2_CC4S;	// Output
	TIM4->CCER 	|= TIM_CCER_CC4E;	// Channel is ON
	TIM4->CR1 	|= TIM_CR1_ARPE;	// Auto reload
	TIM4->EGR 	|= TIM_EGR_UG;		// Reinitialize the counter
	TIM4->SR 	&= TIM_SR_UIF;		// Clear update interrupt flag
	TIM4->BDTR 	|= TIM_BDTR_MOE;	// Main output Enable

	TIM4->CCR2 = 0; 	// Set duty cycle to 0%

	// Init Timer 4 CH3
	TIM4->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;	// PWM mode 1 upcounting
	TIM4->CCMR2 &= ~TIM_CCMR2_OC3M_0;

	TIM4->CCMR2 |= TIM_CCMR2_OC3PE;	// Preload Enable
	TIM4->CCMR2 &= ~TIM_CCMR2_CC3S;	// Output
	TIM4->CCER 	|= TIM_CCER_CC3E;	// Channel is ON

	TIM4->CCR3 = 0; 	// Set duty cycle to 0%

	// Start Timer 4
	TIM4->CR1 |= TIM_CR1_CEN;


}
/* TIM13 init function */
void MX_TIM13_Init(void)
{

  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 8400;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 1000;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_icHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM2 GPIO Configuration    
    PA15     ------> TIM2_CH1
    PB3     ------> TIM2_CH2 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_icHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration    
    PB4     ------> TIM3_CH1
    PB5     ------> TIM3_CH2 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* TIM4 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();
  /* USER CODE BEGIN TIM4_MspInit 1 */

  /* USER CODE END TIM4_MspInit 1 */
  }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM13)
  {
  /* USER CODE BEGIN TIM13_MspInit 0 */

  /* USER CODE END TIM13_MspInit 0 */
    /* TIM13 clock enable */
    __HAL_RCC_TIM13_CLK_ENABLE();

    /* TIM13 interrupt Init */
    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
  /* USER CODE BEGIN TIM13_MspInit 1 */

  /* USER CODE END TIM13_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspPostInit 0 */

  /* USER CODE END TIM4_MspPostInit 0 */
  
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**TIM4 GPIO Configuration    
    PD14     ------> TIM4_CH3
    PD15     ------> TIM4_CH4 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM4_MspPostInit 1 */

  /* USER CODE END TIM4_MspPostInit 1 */
  }

}

void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{

  if(tim_icHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  
    /**TIM2 GPIO Configuration    
    PA15     ------> TIM2_CH1
    PB3     ------> TIM2_CH2 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);

  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_icHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  
    /**TIM3 GPIO Configuration    
    PB4     ------> TIM3_CH1
    PB5     ------> TIM3_CH2 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4|GPIO_PIN_5);

  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM13)
  {
  /* USER CODE BEGIN TIM13_MspDeInit 0 */

  /* USER CODE END TIM13_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM13_CLK_DISABLE();

    /* TIM13 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
  /* USER CODE BEGIN TIM13_MspDeInit 1 */

  /* USER CODE END TIM13_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
// Fonction validée sur NUCLEO F429ZI le 18-09-2018
void F_TIM4_CH3_SetDC(int dc){
	TIM4->CCR3 = dc*22.5; 	// Set duty cycle to 0%
}
// Fonction validée sur NUCLEO F429ZI le 18-09-2018
void F_TIM4_CH4_SetDC(int dc){
	TIM4->CCR4 = dc*22.5; 	// Set duty cycle to 0%
}


/**
 * @brief Initialize Timer 9
 * Timer 9 : Interruption on overflow every 5 ms
 * The Timer 9 is clocked at 50 MHz
 *
 * Vérifié sur carte NUCLEO-F429ZI le 18-09-2018
 */
void Init_Timer9(){

	RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;

	TIM9->CR1 |= TIM_CR1_ARPE;
	TIM9->PSC = 17995 - 1;	// Prescaler value =  2399+1 = 2400 ====> 24MHz /  2400 = 10 KHz
	TIM9->ARR = 50-1;
	TIM9->DIER |= TIM_DIER_UIE;	// Enable interrupt

	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);

	TIM9->CR1 |= TIM_CR1_CEN;
}
/**
 * @brief Initialize Timer 10
 * Timer 10 : Interruption on overflow every 20 ms
 * The Timer 10 is clocked at 24 MHz
 *
 * Vérifié sur carte NUCLEO-F429ZI le 18-09-2018
 *
 */
void Init_Timer10(){

	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	TIM10->CR1 |= TIM_CR1_ARPE;
	TIM10->PSC = 17995-1;	// Prescaler value  24MHz / 4*2400 = 10 KHz / 4 = 50 Hz
	TIM10->ARR = 200-1;
	TIM10->DIER |= TIM_DIER_UIE;	// Enable interrupt

	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

	TIM10->CR1 |= TIM_CR1_CEN;
}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
