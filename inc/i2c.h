/*
 * i2c.h
 *
 *  Created on: 21.09.2017
 *      Author: Lenovo
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f0xx.h"
#include "gpio.h"

/* use I2C peripheral of STM32 */
#define HARDWARE_I2C

/* control state of LDAC pin during transmitting bytes */
#define USE_LDAC		1
#define NOT_USE_LDAC	0

#define ACK		0
#define NACK	1

/* generate STOP or not after end of byte transfer */
#define NOTSTOP_WHEN_SENT	0
#define STOP_WHEN_SENT		1

#ifdef HARDWARE_I2C

/* I2C peripheral*/
#define I2C_PERIPH	I2C1

/* I2C SPEED */
#define I2C_SPEED	1000000

/* STM32 registers' settings for wanted I2C speed */
#if I2C_SPEED==100000
#define PRESC	0xB
#define SCLL	0x13
#define SCLH	0xF
#define SDADEL	0x2
#define SCLDEL	0x4
#elif I2C_SPEED==400000
#define PRESC	0x05
#define SCLL	0x09
#define SCLH	0x03
#define SDADEL	0x03
#define SCLDEL	0x03
#elif I2C_SPEED==1000000
#define PRESC	0x05
#define SCLL	0x03
#define SCLH	0x01
#define SDADEL	0x00
#define SCLDEL	0x01
#endif

void I2C_Config(void);
void I2C_HW_tx(uint8_t i2cAddr, uint8_t *dataTx, uint8_t length,
		uint8_t stopOrNotWhenSent);
void I2C_HW_rx(uint8_t i2cAddr, uint8_t *dataRx, uint8_t length);

/* use I2C software implementation */
#else
#define SDA_HIGH	GPIO_SetBits(I2C1_PORT, I2C1_SDA_PIN)
#define SDA_LOW		GPIO_ResetBits(I2C1_PORT, I2C1_SDA_PIN)
#define SDA_IN		GPIO_ReadInputDataBit(I2C1_PORT, I2C1_SDA_PIN)
#define SCK_HIGH	GPIO_SetBits(I2C1_PORT, I2C1_SCK_PIN)
#define SCK_LOW		GPIO_ResetBits(I2C1_PORT, I2C1_SCK_PIN)
#define SCK_IN		GPIO_ReadInputDataBit(I2C1_PORT, I2C1_SCK_PIN)
#define I2C_DELAY	Delay10Us(1)

void I2C_SW_start(void);
void I2C_SW_stop(void);
uint8_t I2C_SW_rx(uint8_t ack);
uint8_t I2C_SW_tx(uint8_t byteTx, uint8_t useLdac, GPIO_TypeDef* ldacPort,
		uint16_t ldacPin);
#endif

#endif /* I2C_H_ */
