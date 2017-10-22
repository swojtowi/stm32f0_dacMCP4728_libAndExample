/*
 * rcc.c
 *
 *  Created on: 2 maj 2016
 *      Author: Szymon
 */

#include "rcc.h"

void RCC_ConfigMainClocks(void);
void RCC_ConfigLSI(void);
void RCC_ConfigPeripheralClocks(void);

/**
 * @brief RCC configuration
 */
void RCC_Config(void) {
	RCC_ConfigMainClocks();
	RCC_ConfigPeripheralClocks();
	RCC_ConfigLSI();
}

void RCC_ConfigMainClocks(void) {
	RCC_ClocksTypeDef rccClocksTypeDef;
	ErrorStatus HSEStartUpStatus;

	RCC_DeInit();

	/* Switch on HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) {
	}
	/* Wait for HSE start or timeout */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS) {
		FLASH_SetLatency(FLASH_Latency_1);
		FLASH_PrefetchBufferCmd(ENABLE);

		/* AHB 48MHz */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		/* APB 48MHz */
		RCC_PCLKConfig(RCC_HCLK_Div1);

		/* configure PLL */
		RCC_PLLConfig(RCC_PLLSource_HSE, RCC_PLLMul_6);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
		}

		/* set PLL as clock source for System Clock */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while (RCC_GetSYSCLKSource() != 0x08) {
		}
		RCC_GetClocksFreq(&rccClocksTypeDef);
	}
}

void RCC_ConfigLSI(void) {
	/* Enable the LSI oscillator ************************************************/
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) {
	}
}

void RCC_ConfigPeripheralClocks(void) {
	/* enable clock for used peripherals */
	RCC_AHBPeriphClockCmd(
			RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_DMA1
					| RCC_AHBPeriph_DMA2, ENABLE);
	RCC_APB1PeriphClockCmd(
	RCC_APB1Periph_I2C1 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG | RCC_APB2Periph_USART1,
			ENABLE);
}
