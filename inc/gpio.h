/*
 * gpio.h
 *
 *  Created on: 2 maj 2016
 *      Author: Szymon
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f0xx.h"

/* LED port/pin */
#define LED_GREEN_PORT			GPIOA
#define LED_GREEN_PIN 			GPIO_Pin_5
#define LED_GREEN_PIN_SOURCE 	GPIO_PinSource5

/* button port/pin */
#define BUTTON_PORT				GPIOC
#define BUTTON_PIN 				GPIO_Pin_13
#define BUTTON_PIN_SOURCE		GPIO_PinSource13

/* USART port/pin */
#define USART1_PORT GPIOB
#define USART1_RX_PIN GPIO_Pin_7
#define USART1_RX_PIN_SOURCE GPIO_PinSource7
#define USART1_TX_PIN GPIO_Pin_6
#define USART1_TX_PIN_SOURCE GPIO_PinSource6

/* MCP4728 port/pin */
#define I2C1_PORT GPIOA
#define I2C1_SDA_PIN GPIO_Pin_10
#define I2C1_SDA_PIN_SOURCE GPIO_PinSource10
#define I2C1_SCK_PIN GPIO_Pin_9
#define I2C1_SCK_PIN_SOURCE GPIO_PinSource9

#define LDAC1_PORT GPIOA
#define LDAC1_PIN GPIO_Pin_15
#define LDAC1_PIN_SOURCE GPIO_PinSource15

#define LDAC2_PORT GPIOA
#define LDAC2_PIN GPIO_Pin_12
#define LDAC2_PIN_SOURCE GPIO_PinSource12

#define LDAC3_PORT GPIOA
#define LDAC3_PIN GPIO_Pin_11
#define LDAC3_PIN_SOURCE GPIO_PinSource11

#define LDAC4_PORT GPIOA
#define LDAC4_PIN GPIO_Pin_8
#define LDAC4_PIN_SOURCE GPIO_PinSource8

#define READY1_PORT GPIOB
#define READY1_PIN GPIO_Pin_4
#define READY1_PIN_SOURCE GPIO_PinSource4

#define READY2_PORT GPIOB
#define READY2_PIN GPIO_Pin_3
#define READY2_PIN_SOURCE GPIO_PinSource3

#define READY3_PORT GPIOB
#define READY3_PIN GPIO_Pin_1
#define READY3_PIN_SOURCE GPIO_PinSource1

#define READY4_PORT GPIOB
#define READY4_PIN GPIO_Pin_0
#define READY4_PIN_SOURCE GPIO_PinSource0

void GPIO_Config(void);

#endif /* GPIO_H_ */
