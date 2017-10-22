/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32f0xx.h"
#include "main.h"
#include "rcc.h"
#include "gpio.h"
#include "nvic.h"
#include "usart.h"
#include "i2c.h"
#include "mcp4728.h"
#include "timer.h"
#include "dma.h"
#include "iwdg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void SysTimer_Config(uint32_t clkSource, uint32_t ticks);
static void USART_RX_PARSE_EVENT(void);

/* variable being decreased every 10us */
volatile uint32_t uwTimingDelay;

/* flag being set every 1 ms */
volatile uint8_t ms1_flag;

/* flag being set every 1s */
volatile uint8_t s1_flag;

/* text buffer for UART printing */
char textBuf[STR_MAX_LENGTH] = "";

int main(void) {
	/**
	 * @brief uint8_t seconds - variable to store seconds
	 */
	uint8_t seconds = 0;

	SystemInit();
	RCC_Config();
	SystemCoreClockUpdate();
	IWDG_Config();

	/* SysTick configured to overload every 10 us */
	SysTimer_Config(SysTick_CLKSource_HCLK, SYSTICKS);
	GPIO_Config();
	NVIC_Config();
	/*DMA_Config();
	 TIM_Config();*/
	USART_Config();
	MCP4728_Init();
	//setbuf(stdout, NULL);
	while (1) {
		/* executed only every second */
		if (s1_flag) {
			//sprintf(textBuf, "Szymon jest zajebisty %d\n", seconds);
			PRINTF("Szymon jest zajebisty\n");
			USART_RX_PARSE_EVENT();
			seconds++;
			/* executed every odd second */
			if ((seconds % 2) == 0) {
			}
			/* executed every even second */
			else if ((seconds % 2) == 1) {
			}
			if (seconds > 59) {
				seconds = 0;
			}
			IWDG_ReloadCounter();
			s1_flag = 0;
		}
	}
}

/**
 * @brief delay for ... 10us
 * @param <nTime> time in 10us
 * @retval None
 */
void Delay10Us(__IO uint32_t nTime) {
	uwTimingDelay = nTime;
	while (uwTimingDelay != 0)
		;
}

/**
 * @brief delay for ... ms
 * @param <nTime> time in ms
 * @retval None
 */
void DelayMs(__IO uint32_t nTime) {
	while (nTime != 0) {
		if (ms1_flag) {
			nTime--;
			ms1_flag = 0;
		}
	}
}

/**
 * @brief function called every 1us (helpful for delay)
 * @retval None
 */
void TimingDelay_Decrement(void) {
	if (uwTimingDelay != 0x00) {
		uwTimingDelay--;
	}
}

/**
 * @brief SysTick Configuration
 * @param clkSource source of clock
 * @param ticks amount of ticks to overload SysTick
 * @retval None
 */
void SysTimer_Config(uint32_t clkSource, uint32_t ticks) {
	SysTick_CLKSourceConfig(clkSource);
	SysTick_Config(ticks);
}

/* Parsing data received by USART */
static void USART_RX_PARSE_EVENT(void) {
	const char delim[2] = ";";
	char *token;
	uint8_t outputNumber;
	uint32_t valueToSet;

	if (usart1RxCmdFlag) {
		USART_GetBuf(&USART1_RxBuf, textBuf, STR_MAX_LENGTH);

		/* get the first token */
		token = strtok(textBuf, delim);
		if (strncmp(token, "OUTPUT", 6) == 0) {
			outputNumber = atoi(token + 6) - 1;
			if (outputNumber > (MCP4728_AMOUNT - 1)) {
				outputNumber = MCP4728_AMOUNT - 1;
			}
			token = strtok(NULL, delim);
			/* walk through other tokens */
			while (token != NULL) {
				if (strncmp(token, "ResA", 4) == 0) {
					PRINTF("resA ");
					valueToSet = atoi(token + 5);
					mcp4728WholeData[outputNumber].channelA.dacValue = 4095
							* (valueToSet - SERIAL1) / (valueToSet + PULLUP1);
				} else if (strncmp(token, "ResB", 4) == 0) {
					PRINTF("resB ");
					valueToSet = atoi(token + 5);
					mcp4728WholeData[outputNumber].channelB.dacValue = 4095
							* (valueToSet - SERIAL2) / (valueToSet + PULLUP2);
				} else if (strncmp(token, "ResC", 4) == 0) {
					PRINTF("resC ");
					valueToSet = atoi(token + 5);
					mcp4728WholeData[outputNumber].channelC.dacValue = 4095
							* (valueToSet - SERIAL2) / (valueToSet + PULLUP2);
				} else if (strncmp(token, "ResD", 4) == 0) {
					PRINTF("resD ");
					valueToSet = atoi(token + 5);
					mcp4728WholeData[outputNumber].channelD.dacValue = 4095
							* (valueToSet - SERIAL3) / (valueToSet + PULLUP3);
				}
				token = strtok(NULL, delim);
			}
			MCP4728_SequentialWrite(outputNumber, CHANNEL_A);
		}
		usart1RxCmdFlag = 0;
	}
}
