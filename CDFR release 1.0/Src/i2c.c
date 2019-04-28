/**
  ******************************************************************************
  * File Name          : I2C.c
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

}
/* I2C2 init function */
void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */
  
    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**I2C2 GPIO Configuration    
    PF0     ------> I2C2_SDA
    PF1     ------> I2C2_SCL 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();
  
    /**I2C2 GPIO Configuration    
    PF0     ------> I2C2_SDA
    PF1     ------> I2C2_SCL 
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/** I2C2 *****/
/**
 * Write a single value in a slave register
 */
uint8_t F_I2C2_WriteRegister(uint8_t slave_addr, uint16_t register_addr, uint8_t *value, uint8_t size){
	uint8_t i2c_status = I2C_STATUS_OK;
	uint32_t timeout = 0;
    uint8_t i = 0;

    // Send start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB)){	// wait for START condition (SB=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
	timeout++;
	}
	// Send slave address
	I2C2->DR = slave_addr & 0xFE  ;	// address + write
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){// wait for ADDRESS sent (ADDR=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
		timeout++;
	}

	i2c_status = I2C2->SR2; // read status to clear flag

	// Send register address MSB
	I2C2->DR = (uint8_t)((register_addr>>8) & 0x00FF);
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	// Send register address LSB
	I2C2->DR = (uint8_t)(register_addr & 0x00FF);
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	for(i=0;i<size;i++)
	{

		// Send new value to write to the register
		I2C2->DR = value[i];
		while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	}

	I2C2->CR1 |= I2C_CR1_STOP; // send STOP bit
	return i2c_status;
}

/**
 * Read a single value from a slave register
 */
int F_I2C2_ReadRegister(uint8_t slave_addr, uint8_t register_addr,uint8_t register_addr_2, uint8_t *value_read){
	int i2c_status = I2C_STATUS_OK;	// Init return value to error
	int timeout=0;

	// Send start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB)){	// wait for START condition (SB=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
	timeout++;
	}
	// Send slave address
	I2C2->DR = slave_addr & 0xFE  ;	// address + write
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){// wait for ADDRESS sent (ADDR=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
		timeout++;
	}

	i2c_status = I2C2->SR2; // read status to clear flag

	// Send register address
	I2C2->DR = register_addr;
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	// Send register address
	I2C2->DR = register_addr_2;
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)


	// Send repeated start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB));	// wait for START condition (SB=1)

	timeout=0;
	// Send slave address
	I2C2->DR = slave_addr | 1;	// address + read
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){ // wait for ADDRESS sent (ADDR=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
		timeout++;
	}
	i2c_status = I2C2->SR2; // read status to clear flag

	// prepare NACK
	I2C2->CR1 &= ~I2C_CR1_ACK;

	// Wait for Data available
	while (!(I2C2->SR1 & I2C_SR1_RXNE));
	*value_read = I2C2->DR; 			// Address in chip -> DR & write

	// send STOP bit
	I2C2->CR1 |= I2C_CR1_STOP;
	return i2c_status;
}


// Read x value from the device
uint8_t F_I2C2_ReadRegisterVL53L1X(uint8_t slave_addr, uint16_t register_addr, uint8_t nb_value_to_read, uint8_t *value_read){
	uint8_t i2c_status = 0;	// Init return value to error
	uint8_t status = I2C_STATUS_OK;
	int timeout=0;

	uint8_t i = 0;

	// Send start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB)){	// wait for START condition (SB=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
	timeout++;
	}
	// Send slave address
	I2C2->DR = slave_addr & 0xFE  ;	// address + write
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){// wait for ADDRESS sent (ADDR=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
		timeout++;
	}

	i2c_status = I2C2->SR2; // read status to clear flag

	// Send register address MSB
	I2C2->DR = (uint8_t)((register_addr>>8) & 0x00FF);
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)

	// Send register address LSB
	I2C2->DR = (uint8_t)(register_addr & 0x00FF);
	while ((!(I2C2->SR1 & I2C_SR1_TXE)) && (!(I2C2->SR1 & I2C_SR1_BTF))); // wait for DR empty (TxE)


	// Send repeated start
	I2C2->CR1 |= I2C_CR1_START; // send START bit
	while (!(I2C2->SR1 & I2C_SR1_SB));	// wait for START condition (SB=1)

	timeout=0;
	// Send slave address
	I2C2->DR = slave_addr | 1;	// address + read
	while (!(I2C2->SR1 & I2C_SR1_ADDR)){ // wait for ADDRESS sent (ADDR=1)
		/*if(timeout > I2C_TIMEOUT){

			return I2C_STATUS_KO;
		}*/
		timeout++;
	}
	i2c_status = I2C2->SR2; // read status to clear flag


	// prepare ACK
	I2C2->CR1 |= I2C_CR1_ACK;

	for(i=0;i<nb_value_to_read;i++)
	{

		// If this is the last byte to receive
		if((i+1) >= nb_value_to_read)
		{
			// prepare NACK
			I2C2->CR1 &= ~I2C_CR1_ACK;
		}

		// Wait for Data available
			while (!(I2C2->SR1 & I2C_SR1_RXNE));
			value_read[i] = I2C2->DR; 			// Address in chip -> DR & write
	}


	// send STOP bit
	I2C2->CR1 |= I2C_CR1_STOP;
	return status;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
