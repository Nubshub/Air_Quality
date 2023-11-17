/*
 * msp.c
 *
 *  Created on: Nov 15, 2023
 *      Author: admin
 */

#include "main.h"

void HAL_MspInit(void)
{
	// set up the priority grouping of the arm Cortex Mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//enable the required system exceptions of the arm processor
	SCB->SHCSR |= 0x7 << 16; //usage fault, mem fault, bus fault system exceptions

	//configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart2;

	//enable the clock
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//GPIO configuration for UART2
	gpio_uart2.Alternate = GPIO_AF7_USART2;
	gpio_uart2.Pin = GPIO_PIN_2 | GPIO_PIN_3; // PIN2 -> TX, PIN3 -> RX
	gpio_uart2.Mode = GPIO_MODE_AF_PP;
	gpio_uart2.Pull = GPIO_NOPULL;
	gpio_uart2.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio_uart2);

	//enable NVIC
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);


}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef gpio_i2c;

	//enable the clock
	__HAL_RCC_I2C1_CLK_ENABLE();

	//GPIO configuration for I2C1
	gpio_i2c.Alternate = GPIO_AF4_I2C1;
	gpio_i2c.Pin = GPIO_PIN_6 | GPIO_PIN_7; // PIN6 -> SCL, PIN7 -> SDA
	gpio_i2c.Mode = GPIO_MODE_AF_PP;
	gpio_i2c.Pull = GPIO_PULLUP;
	gpio_i2c.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio_i2c);
}










