/*
 * main.c
 *
 *  Created on: Nov 15, 2023
 *      Author: admin
 */


#include "main.h"

UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;

volatile uint32_t ms;
char msg[100];
const uint8_t init_cmd = 0xFF;
const uint8_t start_cmd[5] = {0xFE, 0xA5, 0x00, 0x11, 0xB6};
const uint8_t stop_cmd[5] = {0xFE, 0xA5, 0x00, 0x10, 0xB5};
const uint8_t read_pm25[5] = {0xFE, 0xA5, 0x00, 0x00, 0xA5};
const uint8_t read_all[5] = {0xFE, 0xA5, 0x00, 0x01, 0xA6};

uint8_t value_pm25[11];
uint8_t data;
uint8_t cnt = 0;

int main(void)
{


	HAL_Init();

	SysClockConfig();

	UART2_Init();

	I2C1_Init();

	HAL_UART_Transmit(&huart2, &init_cmd, sizeof(init_cmd), HAL_MAX_DELAY);
	delay_ms(20);
	HAL_UART_Transmit(&huart2, start_cmd, sizeof(start_cmd), HAL_MAX_DELAY);
	delay_ms(2000);
	HAL_UART_Transmit(&huart2, read_all, sizeof(read_all), HAL_MAX_DELAY);

	while(cnt < 11)
		HAL_UART_Receive_IT(&huart2, &data, 1);

	delay_ms(1000);
	HAL_UART_Transmit(&huart2, stop_cmd, sizeof(stop_cmd), HAL_MAX_DELAY);


	sprintf(msg, "\r\n PM1 = %d ug/m3\r\n PM2.5 = %d ug/m3\r\n PM10 = %d ug/m3\r\n", value_pm25[6], value_pm25[8], value_pm25[10]);
	HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);



	while(1);

	return 0;
}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 1200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&huart2);
}
void I2C1_Init(void)
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.OwnAddress1 = 0x61;
	if(HAL_I2C_Init(&hi2c1) != HAL_OK)
		while(1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	value_pm25[cnt++] = data;
}

void delay_ms(uint32_t delay)
{
	ms = 0;
	while(ms < delay);
}

void SysClockConfig(void)
{
	//default CLK 16MHz
}

