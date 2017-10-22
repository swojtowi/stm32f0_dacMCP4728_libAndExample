/*
 * i2c.c
 *
 *  Created on: 21.09.2017
 *      Author: Lenovo
 */

#include "i2c.h"
#include "main.h"
#include "usart.h"

#ifdef HARDWARE_I2C

/**
 * @brief I2C peripheral configuration */
void I2C_Config(void) {
	I2C_InitTypeDef I2C_InitStructure;

	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Timing = (PRESC << 28) | (SCLDEL << 20)
	| (SDADEL << 16) | (SCLH << 8) | SCLL; 		// I2C speed
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;// I2C mode
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;// enable acknowledge when reading
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;// set address length to 7 bit addresses
	I2C_Init(I2C_PERIPH, &I2C_InitStructure);
	I2C_Cmd(I2C_PERIPH, ENABLE);
}

/**
 * @brief I2C transmit amount of bytes
 * @param i2cAddr I2C address
 * @param dataTx address of first byte to transmit
 * @param length amount of bytes to transmit
 * @param stopOrNotWhenSent generate STOP after end of transfer or not
 */
void I2C_HW_tx(uint8_t i2cAddr, uint8_t *dataTx, uint8_t length,
		uint8_t stopOrNotWhenSent) {

	if (stopOrNotWhenSent == STOP_WHEN_SENT) {
		I2C_TransferHandling(I2C_PERIPH, i2cAddr, length,
				I2C_AutoEnd_Mode,
				I2C_Generate_Start_Write);
		//PRINTF("\nI2C_tx - 1\n");
		while (I2C_GetFlagStatus(I2C_PERIPH, I2C_FLAG_TXIS) == RESET)
		;
		//PRINTF("\nI2C_tx - 2\n");
		for (uint8_t i = 0; i < length; i++) {
			while (I2C_GetFlagStatus(I2C_PERIPH, I2C_FLAG_TXE) == RESET)
			;
			I2C_SendData(I2C_PERIPH, *(dataTx + i));
			//PRINTF("\nI2C_tx - 3\n");
		}
		while (I2C_GetFlagStatus(I2C_PERIPH, I2C_FLAG_STOPF) == RESET)
		;
		I2C_ClearFlag(I2C_PERIPH, I2C_FLAG_STOPF);
	} else {
		I2C_TransferHandling(I2C_PERIPH, i2cAddr, length, I2C_SoftEnd_Mode,
				I2C_Generate_Start_Write);
		//PRINTF("\nI2C_tx - 1\n");
		while (I2C_GetFlagStatus(I2C_PERIPH, I2C_FLAG_TXIS) == RESET)
		;
		//PRINTF("\nI2C_tx - 2\n");
		for (uint8_t i = 0; i < length; i++) {
			while (I2C_GetFlagStatus(I2C_PERIPH, I2C_FLAG_TXE) == RESET)
			;
			I2C_SendData(I2C_PERIPH, *(dataTx + i));
			//PRINTF("\nI2C_tx - 3\n");
		}
		while (I2C_GetFlagStatus(I2C_PERIPH, I2C_FLAG_TC) == RESET)
		;
		//PRINTF("\nI2C_tx - 4\n");
	}
	//PRINTF("\nI2C_tx - 5\n");
}

/**
 * @brief I2C receive amount of bytes
 * @param i2cAddr I2C address
 * @param dataRx address of first byte to write received value
 * @param length amount of bytes to receive
 */
void I2C_HW_rx(uint8_t i2cAddr, uint8_t *dataRx, uint8_t length) {

//PRINTF("\nI2C_rx - 1\n");
	I2C_TransferHandling(I2C_PERIPH, i2cAddr, length, I2C_AutoEnd_Mode,
			I2C_Generate_Start_Read);

	for (uint8_t i = 0; i < length; i++) {
		while (I2C_GetFlagStatus(I2C_PERIPH, I2C_FLAG_RXNE) == RESET)
		;
		//PRINTF("\nI2C_rx - 2\n");
		*(dataRx + i) = I2C_ReceiveData(I2C_PERIPH);
	}
	while (I2C_GetFlagStatus(I2C_PERIPH, I2C_FLAG_STOPF) == RESET)
	;
	//PRINTF("\nI2C_rx - 3\n");
	I2C_ClearFlag(I2C_PERIPH, I2C_FLAG_STOPF);
}
#else
/**
 * @brief make I2C software START condition
 */
void I2C_SW_start(void) {
	SDA_HIGH;
	I2C_DELAY;
	SCK_HIGH;
	I2C_DELAY;
	SDA_LOW;
	I2C_DELAY;
	SCK_LOW;
	I2C_DELAY;
	SDA_HIGH;
}

/**
 * @brief make I2C software STOP condition
 */
void I2C_SW_stop(void) {
	SDA_LOW;
	I2C_DELAY;
	SCK_HIGH;
	I2C_DELAY;
	SDA_HIGH;
}

/**
 * @brief I2C receive one byte
 * @param ack ACK or NACK
 * @retval received byte
 */
uint8_t I2C_SW_rx(uint8_t ack) {
	uint8_t byteRx = 0;

	for (uint8_t i = 0; i < 8; i++) {
		byteRx <<= 1;
		I2C_DELAY;
		do {
			SCK_HIGH;
		} while (SCK_IN == 0);    // wait for any SCL clock stretching
		I2C_DELAY;
		if (SDA_IN) {
			byteRx |= 1;
		}
		SCK_LOW;
	}
	I2C_DELAY;
	if (ack == ACK) {
		SDA_LOW;
	} else {
		SDA_HIGH;
	}
	I2C_DELAY;
	SCK_HIGH;
	I2C_DELAY;
	SCK_LOW;
	SDA_HIGH;

	return byteRx;
}

/**
 * @brief I2C transmit one byte
 * @param byteTx byte to transmit
 * @param useLdac change state of LDAC pin during byte transmitting
 * @param ldacPort LDAC port
 * @param ldacPin LDAC pin
 * @retval ACK or NACK
 */
uint8_t I2C_SW_tx(uint8_t byteTx, uint8_t useLdac, GPIO_TypeDef* ldacPort,
		uint16_t ldacPin) {
	uint8_t ackOrNot;

	for (uint8_t i = 8; i; i--) {
		if (byteTx & 0x80) {
			SDA_HIGH;
		} else {
			SDA_LOW;
		}
		I2C_DELAY;
		SCK_HIGH;
		I2C_DELAY;
		byteTx <<= 1;
		SCK_LOW;
	}
	if (useLdac == USE_LDAC) {
		I2C_DELAY;
		GPIO_ResetBits(ldacPort, ldacPin);
		I2C_DELAY;
	}
	SDA_HIGH;
	SCK_HIGH;
	I2C_DELAY;
	ackOrNot = SDA_IN;          // possible ACK bit
	SCK_LOW;

	return ackOrNot;
}
#endif
