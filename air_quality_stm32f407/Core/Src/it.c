/*
 * it.c
 *
 *  Created on: Nov 15, 2023
 *      Author: admin
 */


#include "it.h"

extern uint32_t ms;
extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

void SysTick_Handler(void)
{
	HAL_IncTick(); // increment tick every 1 MS
	HAL_SYSTICK_IRQHandler();
	ms++;
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}

void RTC_Alarm_IRQHandler(void)
{

	HAL_RTC_AlarmIRQHandler(&hrtc);
}
