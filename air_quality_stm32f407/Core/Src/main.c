/*
 * main.c
 *
 *  Created on: Nov 15, 2023
 *      Author: admin
 */


#include <main.h>

UART_HandleTypeDef huart2, huart3;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim6;
GPIO_InitTypeDef pd11;
RTC_HandleTypeDef hrtc;

DHT11_t dht11;
char HUM[100], TEMP[100], CO[100];

volatile uint32_t ms;
char msg[100];
char HUM[100], TEMP[100];

//commands for the particle sensor
const uint8_t init_cmd = 0xFF;
const uint8_t start_cmd[5] = {0xFE, 0xA5, 0x00, 0x11, 0xB6};
const uint8_t stop_cmd[5] = {0xFE, 0xA5, 0x00, 0x10, 0xB5};
const uint8_t read_pm25[5] = {0xFE, 0xA5, 0x00, 0x00, 0xA5};
const uint8_t read_all[5] = {0xFE, 0xA5, 0x00, 0x01, 0xA6};

IH_PMC_t particle_sensor;
uint8_t data_byte;
uint8_t cnt = 0, alarm_cnt = 0;

int main(void)
{
	HAL_Init();

	SysClockConfig();

	UART2_Init();
	UART3_Init();
	TIM6_Init();
	RTC_Init();
	RTC_AlarmConfig();


	while(1)
	{
		if(alarm_cnt)
		{
			RTC_read();
			Particle_sensor_values();
			Temp_humidity_values();
			alarm_cnt = 0;

			HAL_PWR_EnterSTANDBYMode();

		}
	}

	return 0;
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	alarm_cnt++;

}

void RTC_AlarmConfig(void)
{
	RTC_AlarmTypeDef alarm_set;

	memset(&alarm_set, 0 , sizeof(alarm_set));

	//xx:xx:10
	alarm_set.Alarm = RTC_ALARM_A;
	alarm_set.AlarmTime.Seconds = 10;
	alarm_set.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES;
	alarm_set.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
	HAL_RTC_SetAlarm_IT(&hrtc, &alarm_set, RTC_FORMAT_BIN);
}

void RTC_read(void)
{
	RTC_TimeTypeDef rtc_time;
	RTC_DateTypeDef rtc_date;

	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);

	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	sprintf(msg, "Time: %02d:%02d:%02d\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
	HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);


}

void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 249;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	HAL_RTC_Init(&hrtc);
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

void UART3_Init(void)
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 1200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&huart3);
}

void TIM6_Init(void)
{
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 16 - 1; //TIM_CLK = 1MHz -> tick = 1us
	htim6.Init.Period = 0xffff; //MAX ARR value
	HAL_TIM_Base_Init(&htim6);
}

void delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(&htim6, 0); //set counter to 0
	while(__HAL_TIM_GET_COUNTER(&htim6) < us); //wait until reaches the desired delay in us
}


void PD11_Init_Output(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();

	pd11.Mode = GPIO_MODE_OUTPUT_PP;
	pd11.Pin = GPIO_PIN_11;
	pd11.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &pd11);

}
void PD11_Init_Input(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();

	pd11.Mode = GPIO_MODE_INPUT;
	pd11.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOD, &pd11);

}

void DHT11_start(void)
{
	PD11_Init_Output(); 									//PD11 set as output
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET); 	// PD11 set to LOW

	delay_ms(18);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET); 	// PD11 set to HIGH
	PD11_Init_Input(); 										// PD11 set as input

}

uint8_t Check_Response(void)
{
	uint8_t Response = 0;

	delay_us(40);							//40us delay

	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_RESET)		//if PD11 is LOW
	{
		delay_us(80);						//80us delay

		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_SET)
		{
			Response = 1;

		}
		else
		{
			Response = -1;
		}

	}


	while(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_SET); //wait for the PD11 to go LOW


	return Response;
}

void DHT11_read(void)
{

	uint8_t bit;


	for(uint32_t i = 0; i<32; i++)
	{
		while(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_RESET); 	//wait for the PD11 to go HIGH
		delay_us(40);

		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_SET)		//check if the PD11 is HIGH or LOW
			bit = 1;
		else
			bit = 0;

		dht11.getResult |= (bit << (31 - i));


		while(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11) == GPIO_PIN_SET); 	//wait for the PD11 to go LOW

	}

}
void Temp_humidity_values(void)
{
	HAL_TIM_Base_Start(&htim6);//start timer ticking

	delay_ms(1000);

	DHT11_start();
	Check_Response();
	DHT11_read();

	sprintf(HUM, "HUM: %d,%d %%\r\n", dht11.Values.intRH,dht11.Values.decRH );
	HAL_UART_Transmit(&huart3, (uint8_t*)HUM, strlen(HUM), HAL_MAX_DELAY);

	sprintf(TEMP, "TEMP: %d,%d C\r\n\r\n", dht11.Values.intT,dht11.Values.decT);
	HAL_UART_Transmit(&huart3, (uint8_t*)TEMP, strlen(TEMP), HAL_MAX_DELAY);

	dht11.getResult = 0;


}
void Particle_sensor_values(void)
{
	HAL_UART_Transmit(&huart2, &init_cmd, sizeof(init_cmd), HAL_MAX_DELAY);
	delay_ms(20);
	HAL_UART_Transmit(&huart2, start_cmd, sizeof(start_cmd), HAL_MAX_DELAY);
	delay_ms(2000);
	HAL_UART_Transmit(&huart2, read_all, sizeof(read_all), HAL_MAX_DELAY);

	while(cnt < 11)
		HAL_UART_Receive_IT(&huart2, &data_byte, 1);

	delay_ms(1000);
	HAL_UART_Transmit(&huart2, stop_cmd, sizeof(stop_cmd), HAL_MAX_DELAY);


	sprintf(msg, "PM1: %d ug/m3\r\nPM2.5: %d ug/m3\r\nPM10: %d ug/m3\r\n", particle_sensor.Values.B6, particle_sensor.Values.B8, particle_sensor.Values.B10);
	HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	particle_sensor.getResult[cnt++] = data_byte;
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
