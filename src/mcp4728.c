/*
 * mcp4728.c
 *
 *  Created on: 24.09.2017
 *      Author: Lenovo
 */

#include "mcp4728.h"
#include "i2c.h"
#include "usart.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

static void MCP4728_I2CAddressInit(void);
static void MCP4728_LdacReadyInit(void);
static void MCP4728_ChannelsSettingsInit(void);

struct Mcp4728WholeData mcp4728WholeData[MCP4728_AMOUNT];
static char textBuf[50] = "";

/* Initialization of MCP4728 devices */
void MCP4728_Init(void) {
#ifdef HARDWARE_I2C
	I2C_Config();
#endif
	/* Init IDs of MCP4728 devices */
	mcp4728WholeData[MCP4728_ID1].id = MCP4728_ID1;
	mcp4728WholeData[MCP4728_ID2].id = MCP4728_ID2;
	mcp4728WholeData[MCP4728_ID3].id = MCP4728_ID3;
	//mcp4728WholeData[MCP4728_ID4].id = MCP4728_ID4;

	MCP4728_LdacReadyInit();
	MCP4728_I2CAddressInit();
	MCP4728_ChannelsSettingsInit();

	for (uint8_t i = 0; i < MCP4728_AMOUNT; i++) {
		MCP4728_FastWrite(mcp4728WholeData[i].id);
	}
	MCP4728_Update();

	for (uint8_t i = 0; i < MCP4728_AMOUNT; i++) {
		MCP4728_MultiWriteAllChannels(mcp4728WholeData[i].id);
	}
	mcp4728WholeData[MCP4728_ID2].channelA.dacValue = 30 * 4095 / 33;
	MCP4728_MultiWriteOneChannel(MCP4728_ID2, CHANNEL_A);

	mcp4728WholeData[MCP4728_ID1].channelB.dacValue = 12 * 4095 / 33;
	MCP4728_SingleWrite(MCP4728_ID1, CHANNEL_B);
	DelayMs(40);

	mcp4728WholeData[MCP4728_ID3].channelA.dacValue = 30 * 4095 / 33;
	mcp4728WholeData[MCP4728_ID3].channelB.dacValue = 29 * 4095 / 33;
	mcp4728WholeData[MCP4728_ID3].channelC.dacValue = 28 * 4095 / 33;
	mcp4728WholeData[MCP4728_ID3].channelD.dacValue = 27 * 4095 / 33;
	MCP4728_SequentialWrite(MCP4728_ID3, CHANNEL_A);
	DelayMs(40);

	mcp4728WholeData[MCP4728_ID1].channelA.vref = VREF_INTERNAL;
	mcp4728WholeData[MCP4728_ID1].channelB.vref = VREF_EXTERNAL;
	mcp4728WholeData[MCP4728_ID1].channelC.vref = VREF_INTERNAL;
	mcp4728WholeData[MCP4728_ID1].channelD.vref = VREF_EXTERNAL;
	MCP4728_WriteVref(MCP4728_ID1);

	mcp4728WholeData[MCP4728_ID1].channelC.gain = GAIN2;
	MCP4728_WriteGain(MCP4728_ID1);

	mcp4728WholeData[MCP4728_ID1].channelA.powerMode = POWERDOWN_MODE3;
	MCP4728_WritePowerMode(MCP4728_ID1);

	MCP4728_ReadConfig(MCP4728_ID1);
	/*uint8_t * ptr = mcp4728WholeData[MCP4728_ID1].internalContent;
	 for (uint8_t i = 0; i < 4 * 2 * 3; i++) {
	 itoa(*ptr++, textBuf, 10);
	 PRINTF(textBuf);
	 PRINTF("\n");
	 }*/
	itoa(MCP4728_GetDacValue(MCP4728_ID1, CHANNEL_B, EEPROM), textBuf, 10);
	PRINTF(" ");
	PRINTF(textBuf);
	PRINTF("\n");

	/* Reset settings of MCP4728 devices after show */
	MCP4728_ChannelsSettingsInit();
}

/* LDAC and READY port/pin initialization of MCP4728 devices */
static void MCP4728_LdacReadyInit(void) {
	mcp4728WholeData[MCP4728_ID1].ldacPort = LDAC1_PORT;
	mcp4728WholeData[MCP4728_ID1].ldacPin = LDAC1_PIN;
	mcp4728WholeData[MCP4728_ID1].readyPort = READY1_PORT;
	mcp4728WholeData[MCP4728_ID1].readyPin = READY1_PIN;

	mcp4728WholeData[MCP4728_ID2].ldacPort = LDAC2_PORT;
	mcp4728WholeData[MCP4728_ID2].ldacPin = LDAC2_PIN;
	mcp4728WholeData[MCP4728_ID2].readyPort = READY2_PORT;
	mcp4728WholeData[MCP4728_ID2].readyPin = READY2_PIN;

	mcp4728WholeData[MCP4728_ID3].ldacPort = LDAC3_PORT;
	mcp4728WholeData[MCP4728_ID3].ldacPin = LDAC3_PIN;
	mcp4728WholeData[MCP4728_ID3].readyPort = READY3_PORT;
	mcp4728WholeData[MCP4728_ID3].readyPin = READY3_PIN;

	/*mcp4728WholeData[MCP4728_ID4].ldacPort = LDAC4_PORT;
	 mcp4728WholeData[MCP4728_ID4].ldacPin = LDAC4_PIN;
	 mcp4728WholeData[MCP4728_ID4].readyPort = READY4_PORT;
	 mcp4728WholeData[MCP4728_ID4].readyPin = READY4_PIN;*/
}

/* I2C address initialization of MCP4728 devices */
static void MCP4728_I2CAddressInit(void) {
	mcp4728WholeData[MCP4728_ID1].wantedI2CAddr = MCP4728_ID1_ADDR;
	mcp4728WholeData[MCP4728_ID2].wantedI2CAddr = MCP4728_ID2_ADDR;
	mcp4728WholeData[MCP4728_ID3].wantedI2CAddr = MCP4728_ID3_ADDR;
	//mcp4728WholeData[MCP4728_ID4].wantedI2CAddr = MCP4728_ID4_ADDR;

	for (uint8_t i = 0; i < MCP4728_AMOUNT; i++) {
		mcp4728WholeData[i].currI2CAddr = MCP4728_ReadAddress(
				mcp4728WholeData[i].id) >> 5;
		itoa(mcp4728WholeData[i].currI2CAddr, textBuf, 10);
		PRINTF(" ");
		PRINTF(textBuf);
		if (mcp4728WholeData[i].currI2CAddr
				!= mcp4728WholeData[i].wantedI2CAddr) {
			MCP4728_WriteAddress(mcp4728WholeData[i].id);
		}
	}
}

/* Initial channel settings of MCP4728 devices */
static void MCP4728_ChannelsSettingsInit(void) {
	for (uint8_t i = 0; i < MCP4728_AMOUNT; i++) {
		mcp4728WholeData[i].channelA.channelSelect = CHANNEL_A;
		mcp4728WholeData[i].channelA.dacValue = 4095;
		mcp4728WholeData[i].channelA.gain = GAIN1;
		mcp4728WholeData[i].channelA.powerMode = NORMAL_MODE;
		mcp4728WholeData[i].channelA.vref = VREF_EXTERNAL;

		mcp4728WholeData[i].channelB.channelSelect = CHANNEL_B;
		mcp4728WholeData[i].channelB.dacValue = 25 * 4095 / 33;
		mcp4728WholeData[i].channelB.gain = GAIN1;
		mcp4728WholeData[i].channelB.powerMode = NORMAL_MODE;
		mcp4728WholeData[i].channelB.vref = VREF_EXTERNAL;

		mcp4728WholeData[i].channelC.channelSelect = CHANNEL_C;
		mcp4728WholeData[i].channelC.dacValue = 16 * 4095 / 33;
		mcp4728WholeData[i].channelC.gain = GAIN1;
		mcp4728WholeData[i].channelC.powerMode = NORMAL_MODE;
		mcp4728WholeData[i].channelC.vref = VREF_EXTERNAL;

		mcp4728WholeData[i].channelD.channelSelect = CHANNEL_D;
		mcp4728WholeData[i].channelD.dacValue = 10 * 4095 / 33;
		mcp4728WholeData[i].channelD.gain = GAIN1;
		mcp4728WholeData[i].channelD.powerMode = NORMAL_MODE;
		mcp4728WholeData[i].channelD.vref = VREF_EXTERNAL;
	}
}

/* General call for MCP4728 devices */
uint8_t general_call(uint8_t code) {
#ifdef HARDWARE_I2C
	uint8_t dataTx = code;

	I2C_HW_tx(GENERAL_CALL, &dataTx, 1, STOP_WHEN_SENT);

	return ACK;
#else
	I2C_SW_start();

	if (I2C_SW_tx(GENERAL_CALL, NOT_USE_LDAC, 0, 0) != ACK) {
		goto error;
	}
	if (I2C_SW_tx(code, NOT_USE_LDAC, 0, 0) != ACK) {
		goto error;
	}
	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/* Reset all MCP4728 devices */
uint8_t MCP4728_Reset(void) {
	return general_call(RESET);
}

/* WakeUp all MCP4728 devices */
uint8_t MCP4728_WakeUp(void) {
	return general_call(WAKEUP);
}

/* Update all MCP4728 devices' outputs from their input registers */
uint8_t MCP4728_Update(void) {
	return general_call(UPDATE);
}

/**
 * @brief Read I2C address of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 * @retval I2C address of MCP4728 device
 */
uint8_t MCP4728_ReadAddress(uint8_t mcp4728Id) {
	GPIO_TypeDef * ldacPort = mcp4728WholeData[mcp4728Id].ldacPort;
	uint16_t ldacPin = mcp4728WholeData[mcp4728Id].ldacPin;

	GPIO_ResetBits(ldacPort, ldacPin);

#ifdef HARDWARE_I2C
	uint8_t dataTx = READADDRESS;
	uint8_t currI2CAddr = 0xFF;

	I2C_HW_tx(GENERAL_CALL, &dataTx, 1, NOTSTOP_WHEN_SENT);
	I2C_HW_rx(READRESTART, &currI2CAddr, 1);
	GPIO_SetBits(ldacPort, ldacPin);

	return currI2CAddr;
#else
// Generate START condition
	I2C_SW_start();
// Write General Call
	if (I2C_SW_tx(GENERAL_CALL, NOT_USE_LDAC, 0, 0) != ACK) {
		goto error;
	}
// Write old address with LDAC transition
	if (I2C_SW_tx(READADDRESS, NOT_USE_LDAC, 0, 0) != ACK) {
		goto error;
	}
// Generate RESTART condition
	I2C_SW_start();
// Write restart byte
	if (I2C_SW_tx(READRESTART, NOT_USE_LDAC, 0, 0) != ACK) {
		goto error;
	}
	GPIO_SetBits(ldacPort, ldacPin);
// Read response
	uint8_t currI2CAddr = I2C_SW_rx(NACK);
	I2C_SW_stop();
	return currI2CAddr;

	error: GPIO_SetBits(ldacPort, ldacPin);
	I2C_SW_stop();
	return 0xFF;
#endif
}

/**
 * @brief Write I2C address of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 */
uint8_t MCP4728_WriteAddress(uint8_t mcp4728Id) {
	GPIO_TypeDef * ldacPort = mcp4728WholeData[mcp4728Id].ldacPort;
	uint16_t ldacPin = mcp4728WholeData[mcp4728Id].ldacPin;
	uint8_t currI2CAddr = mcp4728WholeData[mcp4728Id].currI2CAddr;
	uint8_t wantedI2CAddr = mcp4728WholeData[mcp4728Id].wantedI2CAddr;

	GPIO_SetBits(ldacPort, ldacPin);

#ifdef HARDWARE_I2C
	/*uint8_t dataTx = WRITECURRENT | (currI2CAddr << 2);

	 I2C_HW_tx(DEVICE_CODE | (currI2CAddr << 1), &dataTx, 1, NOTSTOP_WHEN_SENT);
	 dataTx = WRITECONFIRM | (wantedI2CAddr << 2);
	 I2C_HW_tx(WRITENEW | (wantedI2CAddr << 2), &dataTx, 1, STOP_WHEN_SENT);
	 GPIO_SetBits(ldacPort, ldacPin);*/

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
// Write device code with current address bits
	if (I2C_SW_tx(DEVICE_CODE | (currI2CAddr << 1), NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWriteAddress - blad1\n");
		goto error;
	}
// Write command type with current address bits and LDAC strobe
	if (I2C_SW_tx(WRITECURRENT | (currI2CAddr << 2), USE_LDAC, ldacPort,
					ldacPin) != ACK) {
		PRINTF("\nWriteAddress - blad2\n");
		goto error;
	}
// Write command type with new address bits
	if (I2C_SW_tx(WRITENEW | (wantedI2CAddr << 2), NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWriteAddress - blad3\n");
		goto error;
	}
// Write command type with new address confirmation
	if (I2C_SW_tx(WRITECONFIRM | (wantedI2CAddr << 2), NOT_USE_LDAC, 0,
					0) != ACK) {
		PRINTF("\nWriteAddress - blad4\n");
		goto error;
	}
	GPIO_SetBits(ldacPort, ldacPin);
	I2C_SW_stop();
	return ACK;

	error: GPIO_SetBits(ldacPort, ldacPin);
	I2C_SW_stop();
	return NACK;
#endif
}

/**
 * @brief FastWrite all channels' data to single MCP4728 device, EEPROM is not affected
 * @param mcp4728Id id of MCP4728 device
 */
uint8_t MCP4728_FastWrite(uint8_t mcp4728Id) {
	struct Mcp4728SingleChannelData * channelData =
			&mcp4728WholeData[mcp4728Id].channelA;

#ifdef HARDWARE_I2C
	uint8_t dataTx[8];

	for (uint8_t i = 0; i < 4; i++) {
		*(dataTx + i * 2) =
				FASTWRITE
						| (((channelData->powerMode << 4)
								| (channelData->dacValue >> 8)) & 0x3F);
		*(dataTx + i * 2 + 1) = channelData->dacValue & 0xFF;
		if (i < 3) {
			channelData++;
		}
	}
	I2C_HW_tx(DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
			dataTx, 8, STOP_WHEN_SENT);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nFastWrite - blad1\n");
		goto error;
	}
	for (uint8_t i = 0; i < 4; i++) {
		if (I2C_SW_tx(
						FASTWRITE
						| (((channelData->powerMode << 4)
										| (channelData->dacValue >> 8)) & 0x3F),
						NOT_USE_LDAC, 0, 0) != ACK) {
			PRINTF("\nFastWrite - blad2\n");
			goto error;
		}
		if (I2C_SW_tx(channelData->dacValue & 0xFF,
						NOT_USE_LDAC, 0, 0) != ACK) {
			PRINTF("\nFastWrite - blad3\n");
			goto error;
		}
		if (i < 3) {
			channelData++;
		}
	}

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/**
 * @brief MultiWrite all channels' data to single MCP4728 device, EEPROM is not affected
 * @param mcp4728Id id of MCP4728 device
 */
uint8_t MCP4728_MultiWriteAllChannels(uint8_t mcp4728Id) {
	struct Mcp4728SingleChannelData * channelData =
			&mcp4728WholeData[mcp4728Id].channelA;

#ifdef HARDWARE_I2C
	uint8_t dataTx[12];

	for (uint8_t i = 0; i < 4; i++) {
		*(dataTx + i * 3) =
		MULTIWRITE | (((channelData->channelSelect << 1) | UDAC_LOW) & 0x07);
		*(dataTx + i * 3 + 1) = (channelData->vref << 7)
				| (channelData->powerMode << 5) | (channelData->gain << 4)
				| ((channelData->dacValue >> 8) & 0xFF);
		*(dataTx + i * 3 + 2) = channelData->dacValue & 0xFF;
		if (i < 3) {
			channelData++;
		}
	}
	I2C_HW_tx(DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
			dataTx, 12, STOP_WHEN_SENT);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nMultiWriteAllChannels - blad1\n");
		goto error;
	}
	for (uint8_t i = 0; i < 4; i++) {
		if (I2C_SW_tx(
						MULTIWRITE | (((channelData->channelSelect << 1) | UDAC_LOW) & 0x07),
						NOT_USE_LDAC, 0, 0) != ACK) {
			PRINTF("\nMultiWriteAllChannels - blad2\n");
			goto error;
		}
		if (I2C_SW_tx(
						(channelData->vref << 7) | (channelData->powerMode << 5)
						| (channelData->gain << 4)
						| ((channelData->dacValue >> 8) & 0xFF),
						NOT_USE_LDAC, 0, 0) != ACK) {
			PRINTF("\nMultiWriteAllChannels - blad3\n");
			goto error;
		}
		if (I2C_SW_tx(channelData->dacValue & 0xFF,
						NOT_USE_LDAC, 0, 0) != ACK) {
			PRINTF("\nMultiWriteAllChannels - blad4\n");
			goto error;
		}
		if (i < 3) {
			channelData++;
		}
	}

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/**
 * @brief MultiWrite single channel's data to single MCP4728 device, EEPROM is not affected
 * @param mcp4728Id id of MCP4728 device
 * @param channel channel of data we want to transmit
 */
uint8_t MCP4728_MultiWriteOneChannel(uint8_t mcp4728Id, uint8_t channel) {
	struct Mcp4728SingleChannelData * channelData;

	switch (channel) {
	case CHANNEL_A:
		channelData = &mcp4728WholeData[mcp4728Id].channelA;
		break;
	case CHANNEL_B:
		channelData = &mcp4728WholeData[mcp4728Id].channelB;
		break;
	case CHANNEL_C:
		channelData = &mcp4728WholeData[mcp4728Id].channelC;
		break;
	case CHANNEL_D:
		channelData = &mcp4728WholeData[mcp4728Id].channelD;
		break;
	default:
		return NACK;
	}

#ifdef HARDWARE_I2C
	uint8_t dataTx[3];

	*(dataTx) =
	MULTIWRITE | (((channelData->channelSelect << 1) | UDAC_LOW) & 0x07);
	*(dataTx + 1) = (channelData->vref << 7) | (channelData->powerMode << 5)
			| (channelData->gain << 4) | ((channelData->dacValue >> 8) & 0xFF);
	*(dataTx + 2) = channelData->dacValue & 0xFF;
	I2C_HW_tx(DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
			dataTx, 3, STOP_WHEN_SENT);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nMultiWriteOneChannel - blad1\n");
		goto error;
	}
	if (I2C_SW_tx(
					MULTIWRITE | (((channelData->channelSelect << 1) | UDAC_LOW) & 0x07),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nMultiWriteOneChannel - blad2\n");
		goto error;
	}
	if (I2C_SW_tx(
					(channelData->vref << 7) | (channelData->powerMode << 5)
					| (channelData->gain << 4)
					| ((channelData->dacValue >> 8) & 0xFF),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nMultiWriteOneChannel - blad3\n");
		goto error;
	}
	if (I2C_SW_tx(channelData->dacValue & 0xFF,
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nMultiWriteOneChannel - blad4\n");
		goto error;
	}

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/**
 * @brief SingleWrite single channel's data to single MCP4728 device, EEPROM is affected
 * @param mcp4728Id id of MCP4728 device
 * @param channel channel of data we want to transmit
 */
uint8_t MCP4728_SingleWrite(uint8_t mcp4728Id, uint8_t channel) {
	struct Mcp4728SingleChannelData * channelData;

	switch (channel) {
	case CHANNEL_A:
		channelData = &mcp4728WholeData[mcp4728Id].channelA;
		break;
	case CHANNEL_B:
		channelData = &mcp4728WholeData[mcp4728Id].channelB;
		break;
	case CHANNEL_C:
		channelData = &mcp4728WholeData[mcp4728Id].channelC;
		break;
	case CHANNEL_D:
		channelData = &mcp4728WholeData[mcp4728Id].channelD;
		break;
	default:
		return NACK;
	}

#ifdef HARDWARE_I2C
	uint8_t dataTx[3];

	*(dataTx) =
	SINGLEWRITE | (((channelData->channelSelect << 1) | UDAC_LOW) & 0x07);
	*(dataTx + 1) = (channelData->vref << 7) | (channelData->powerMode << 5)
			| (channelData->gain << 4) | ((channelData->dacValue >> 8) & 0xFF);
	*(dataTx + 2) = channelData->dacValue & 0xFF;
	I2C_HW_tx(DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
			dataTx, 3, STOP_WHEN_SENT);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nSingleWrite - blad1\n");
		goto error;
	}
	if (I2C_SW_tx(
					SINGLEWRITE | (((channelData->channelSelect << 1) | UDAC_LOW) & 0x07),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nSingleWrite - blad2\n");
		goto error;
	}
	if (I2C_SW_tx(
					(channelData->vref << 7) | (channelData->powerMode << 5)
					| (channelData->gain << 4)
					| ((channelData->dacValue >> 8) & 0xFF),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nSingleWrite - blad3\n");
		goto error;
	}
	if (I2C_SW_tx(channelData->dacValue & 0xFF,
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nSingleWrite - blad4\n");
		goto error;
	}

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/*
 * @brief SequentialWrite all channels' data to single MCP4728 device, EEPROM is affected
 * @param mcp4728Id id of MCP4728 device
 * @param startChannel channel from which we start data writing
 */
uint8_t MCP4728_SequentialWrite(uint8_t mcp4728Id, uint8_t startChannel) {
	struct Mcp4728SingleChannelData * channelData;
	uint8_t length;

	switch (startChannel) {
	case CHANNEL_A:
		channelData = &mcp4728WholeData[mcp4728Id].channelA;
		length = 4;
		break;
	case CHANNEL_B:
		channelData = &mcp4728WholeData[mcp4728Id].channelB;
		length = 3;
		break;
	case CHANNEL_C:
		channelData = &mcp4728WholeData[mcp4728Id].channelC;
		length = 2;
		break;
	case CHANNEL_D:
		channelData = &mcp4728WholeData[mcp4728Id].channelD;
		length = 1;
		break;
	default:
		return NACK;
	}

#ifdef HARDWARE_I2C
	uint8_t dataTx[length * 2 + 1];

	*(dataTx) = SEQWRITE
			| (((channelData->channelSelect << 1) | UDAC_LOW) & 0x07);
	for (uint8_t i = 0; i < length; i++) {
		*(dataTx + i * 2 + 1) = (channelData->vref << 7)
				| (channelData->powerMode << 5) | (channelData->gain << 4)
				| ((channelData->dacValue >> 8) & 0xFF);
		*(dataTx + i * 2 + 2) = channelData->dacValue & 0xFF;
		if (i < (length - 1)) {
			channelData++;
		}
	}
	I2C_HW_tx(DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
			dataTx, length * 2 + 1, STOP_WHEN_SENT);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nSequentialWrite - blad1\n");
		goto error;
	}
	if (I2C_SW_tx(
					SEQWRITE | (((channelData->channelSelect << 1) | UDAC_LOW) & 0x07),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nSequentialWrite - blad2\n");
		goto error;
	}
	for (uint8_t i = 0; i < length; i++) {
		if (I2C_SW_tx(
						(channelData->vref << 7) | (channelData->powerMode << 5)
						| (channelData->gain << 4)
						| ((channelData->dacValue >> 8) & 0xFF),
						NOT_USE_LDAC, 0, 0) != ACK) {
			PRINTF("\nSequentialWrite - blad3\n");
			goto error;
		}
		if (I2C_SW_tx(channelData->dacValue & 0xFF,
						NOT_USE_LDAC, 0, 0) != ACK) {
			PRINTF("\nSequentialWrite - blad4\n");
			goto error;
		}
		if (i < (length - 1)) {
			channelData++;
		}
	}

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/*
 * @brief Write VREF settings to single MCP4728 device, EEPROM is not affected
 * @param mcp4728Id id of MCP4728 device
 */
uint8_t MCP4728_WriteVref(uint8_t mcp4728Id) {

#ifdef HARDWARE_I2C
	uint8_t dataTx = WRITEVREF
			| (mcp4728WholeData[mcp4728Id].channelA.vref << 3)
			| (mcp4728WholeData[mcp4728Id].channelB.vref << 2)
			| (mcp4728WholeData[mcp4728Id].channelC.vref << 1)
			| mcp4728WholeData[mcp4728Id].channelD.vref;

	I2C_HW_tx(DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
			&dataTx, 1, STOP_WHEN_SENT);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWriteVref - blad1\n");
		goto error;
	}
	if (I2C_SW_tx(
					WRITEVREF | (mcp4728WholeData[mcp4728Id].channelA.vref << 3)
					| (mcp4728WholeData[mcp4728Id].channelB.vref << 2)
					| (mcp4728WholeData[mcp4728Id].channelC.vref << 1)
					| mcp4728WholeData[mcp4728Id].channelD.vref,
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWriteVref - blad2\n");
		goto error;
	}

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/**
 * @brief Write PowerMode settings to single MCP4728 device, EEPROM is not affected
 * @param mcp4728Id id of MCP4728 device
 */
uint8_t MCP4728_WritePowerMode(uint8_t mcp4728Id) {

#ifdef HARDWARE_I2C
	uint8_t dataTx[2];

	*(dataTx) = WRITEPOWERMODE
			| (mcp4728WholeData[mcp4728Id].channelA.powerMode << 2)
			| mcp4728WholeData[mcp4728Id].channelB.powerMode;
	*(dataTx + 1) = (mcp4728WholeData[mcp4728Id].channelC.powerMode << 6)
			| (mcp4728WholeData[mcp4728Id].channelD.powerMode << 4);

	I2C_HW_tx(DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
			dataTx, 2, STOP_WHEN_SENT);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWritePowerMode - blad1\n");
		goto error;
	}
	if (I2C_SW_tx(
					WRITEPOWERMODE
					| (mcp4728WholeData[mcp4728Id].channelA.powerMode << 2)
					| mcp4728WholeData[mcp4728Id].channelB.powerMode,
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWritePowerMode - blad2\n");
		goto error;
	}
	if (I2C_SW_tx(
					(mcp4728WholeData[mcp4728Id].channelC.powerMode << 6)
					| (mcp4728WholeData[mcp4728Id].channelD.powerMode << 4),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWritePowerMode - blad3\n");
		goto error;
	}

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/**
 * @brief Write GAIN settings to single MCP4728 device, EEPROM is not affected
 * @param mcp4728Id id of MCP4728 device
 */
uint8_t MCP4728_WriteGain(uint8_t mcp4728Id) {

#ifdef HARDWARE_I2C
	uint8_t dataTx = WRITEGAIN
			| (mcp4728WholeData[mcp4728Id].channelA.gain << 3)
			| (mcp4728WholeData[mcp4728Id].channelB.gain << 2)
			| (mcp4728WholeData[mcp4728Id].channelC.gain << 1)
			| mcp4728WholeData[mcp4728Id].channelD.gain;

	I2C_HW_tx(DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
			&dataTx, 1, STOP_WHEN_SENT);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1),
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWriteGain - blad1\n");
		goto error;
	}
	if (I2C_SW_tx(
					WRITEGAIN | (mcp4728WholeData[mcp4728Id].channelA.gain << 3)
					| (mcp4728WholeData[mcp4728Id].channelB.gain << 2)
					| (mcp4728WholeData[mcp4728Id].channelC.gain << 1)
					| mcp4728WholeData[mcp4728Id].channelD.gain,
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nWriteGain - blad2\n");
		goto error;
	}

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/**
 * @brief Read configuration of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 */
uint8_t MCP4728_ReadConfig(uint8_t mcp4728Id) {
	uint8_t *ptr = (uint8_t*) mcp4728WholeData[mcp4728Id].internalContent;

#ifdef HARDWARE_I2C
	I2C_HW_rx(
	DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1) | 0x01, ptr,
			4 * 2 * 3);

	return ACK;
#else
// Generate START condition
	I2C_SW_start();
	if (I2C_SW_tx(
					DEVICE_CODE | (mcp4728WholeData[mcp4728Id].currI2CAddr << 1) | 0x01,
					NOT_USE_LDAC, 0, 0) != ACK) {
		PRINTF("\nReadConfig - blad1\n");
		goto error;
	}
	for (uint8_t i = 1; i < (4 * 2 * 3); i++) {
		*ptr++ = I2C_SW_rx(ACK);
	}
	*ptr++ = I2C_SW_rx(NACK);

	I2C_SW_stop();
	return ACK;

	error: I2C_SW_stop();
	return NACK;
#endif
}

/**
 * @brief Read READY configuration of single channel of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 * @param channel channel of data we want to know
 * @param inputRegisterOrEeprom get configuration of InputRegister or EEPROM
 */
uint8_t MCP4728_GetReady(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom) {
	return (mcp4728WholeData[mcp4728Id].internalContent[channel][inputRegisterOrEeprom][0]
			& 0x80) >> 7;
}

/**
 * @brief Read POR configuration of single channel of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 * @param channel channel of data we want to know
 * @param inputRegisterOrEeprom get configuration of InputRegister or EEPROM
 */
uint8_t MCP4728_GetPOR(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom) {
	return (mcp4728WholeData[mcp4728Id].internalContent[channel][inputRegisterOrEeprom][0]
			& 0x40) >> 6;
}

/**
 * @brief Read VREF configuration of single channel of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 * @param channel channel of data we want to know
 * @param inputRegisterOrEeprom get configuration of InputRegister or EEPROM
 */
uint8_t MCP4728_GetVREF(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom) {
	return (mcp4728WholeData[mcp4728Id].internalContent[channel][inputRegisterOrEeprom][1]
			& 0x80) >> 7;
}

/**
 * @brief Read POWER_DOWN_MODE configuration of single channel of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 * @param channel channel of data we want to know
 * @param inputRegisterOrEeprom get configuration of InputRegister or EEPROM
 */
uint8_t MCP4728_GetPowerMode(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom) {
	return (mcp4728WholeData[mcp4728Id].internalContent[channel][inputRegisterOrEeprom][1]
			& 0x60) >> 5;
}

/**
 * @brief Read GAIN configuration of single channel of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 * @param channel channel of data we want to know
 * @param inputRegisterOrEeprom get configuration of InputRegister or EEPROM
 */
uint8_t MCP4728_GetGain(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom) {
	return (mcp4728WholeData[mcp4728Id].internalContent[channel][inputRegisterOrEeprom][1]
			& 0x10) >> 4;
}

/**
 * @brief Read DAC value of single channel of single MCP4728 device
 * @param mcp4728Id id of MCP4728 device
 * @param channel channel of data we want to know
 * @param inputRegisterOrEeprom get configuration of InputRegister or EEPROM
 */
uint16_t MCP4728_GetDacValue(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom) {
	uint8_t *ptr =
			&(mcp4728WholeData[mcp4728Id].internalContent[channel][inputRegisterOrEeprom][1]);
	uint16_t dacValue = (*ptr++ & 0x0F) << 8;
	dacValue |= *ptr;

	return dacValue;
}
