/*
 * gpio.c
 *
 *  Created on: 2 maj 2016
 *      Author: Szymon
 */

#include "gpio.h"
#include "i2c.h"

/**
 * @brief GPIO configuration
 */
void GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* blue button on Discovery board */
	/*GPIO_StructInit(&GPIO_InitStructure);
	 GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);*/

	/* USART1 */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN | USART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(USART1_PORT, USART1_TX_PIN_SOURCE, GPIO_AF_0);
	GPIO_PinAFConfig(USART1_PORT, USART1_RX_PIN_SOURCE, GPIO_AF_0);

	/* MCP4728 */
	/* I2C1 */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN | I2C1_SCK_PIN;
#ifdef HARDWARE_I2C
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
#else
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
#endif
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C1_PORT, &GPIO_InitStructure);
#ifdef HARDWARE_I2C
	GPIO_PinAFConfig(I2C1_PORT, I2C1_SDA_PIN_SOURCE, GPIO_AF_4);
	GPIO_PinAFConfig(I2C1_PORT, I2C1_SCK_PIN_SOURCE, GPIO_AF_4);
#endif

	/* LDAC */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LDAC1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LDAC1_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LDAC1_PORT, LDAC1_PIN);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LDAC2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LDAC2_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LDAC2_PORT, LDAC2_PIN);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LDAC3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LDAC3_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LDAC3_PORT, LDAC3_PIN);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LDAC4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LDAC4_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LDAC4_PORT, LDAC4_PIN);

	/* READY */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = READY1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(READY1_PORT, &GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = READY2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(READY2_PORT, &GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = READY3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(READY3_PORT, &GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = READY4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(READY4_PORT, &GPIO_InitStructure);
}
