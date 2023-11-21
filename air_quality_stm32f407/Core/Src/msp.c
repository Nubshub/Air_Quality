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
	GPIO_InitTypeDef gpio_uart2, gpio_uart3;

	//enable the clock
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	//GPIO configuration for UART2
	gpio_uart2.Alternate = GPIO_AF7_USART2;
	gpio_uart2.Pin = GPIO_PIN_2 | GPIO_PIN_3; // PIN2 -> TX, PIN3 -> RX
	gpio_uart2.Mode = GPIO_MODE_AF_PP;
	gpio_uart2.Pull = GPIO_NOPULL;
	gpio_uart2.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio_uart2);

	//GPIO configuration for UART3
	gpio_uart3.Alternate = GPIO_AF7_USART1;
	gpio_uart3.Pin = GPIO_PIN_10 | GPIO_PIN_11; // PIN10 -> TX, PIN11 -> RX
	gpio_uart3.Mode = GPIO_MODE_AF_PP;
	gpio_uart3.Pull = GPIO_NOPULL;
	gpio_uart3.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &gpio_uart3);

	//enable NVIC
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);


}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//enable the clock
	__HAL_RCC_TIM6_CLK_ENABLE();
}
