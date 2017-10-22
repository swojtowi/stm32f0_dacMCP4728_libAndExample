/*
 * mcp4728.h
 *
 *  Created on: 24.09.2017
 *      Author: Lenovo
 */

#ifndef MCP4728_H_
#define MCP4728_H_

#include "stm32f0xx.h"
#include "i2c.h"

/* Amount of MCP4728 devices on I2C bus */
#define MCP4728_AMOUNT		3

/* IDs of MCP4728 devices. Should start from 0 */
#define MCP4728_ID1	0
#define MCP4728_ID2	1
#define MCP4728_ID3	2
#define MCP4728_ID4	3

/* I2C addresses of MCP4728 devices */
#define MCP4728_ID1_ADDR	0x00
#define MCP4728_ID2_ADDR	0x01
#define MCP4728_ID3_ADDR	0x02
#define MCP4728_ID4_ADDR	0x03

/* I2C commands for MCP4728 device */
#define DEVICE_CODE		0xC0U

#define GENERAL_CALL 	0x00U
#define RESET			0x06U
#define WAKEUP			0x09U
#define UPDATE			0x08U
#define READADDRESS		0x0CU
#define READRESTART		0xC1U

#define FASTWRITE		0x00U
#define MULTIWRITE		0x40U
#define SEQWRITE		0x50U
#define SINGLEWRITE		0x58U

#define WRITECURRENT	0x61U
#define WRITENEW		0x62U
#define WRITECONFIRM	0x63U

#define WRITEVREF		0x80U
#define WRITEPOWERMODE	0xA0U
#define WRITEGAIN		0xC0U

#define NORMAL_MODE			0x00
#define POWERDOWN_MODE1		0x01
#define POWERDOWN_MODE2		0x02
#define POWERDOWN_MODE3		0x03

#define VREF_EXTERNAL	0x00
#define VREF_INTERNAL	0x01

#define GAIN1	0x00
#define GAIN2	0x01

#define CHANNEL_A	0x00
#define CHANNEL_B	0x01
#define CHANNEL_C	0x02
#define CHANNEL_D	0x03

#define UDAC_HIGH	1
#define UDAC_LOW	0

#define INPUT_REGISTER	0
#define EEPROM			1

/* Single channel data of MCP4728 device */
struct Mcp4728SingleChannelData {
	uint8_t channelSelect;
	uint16_t dacValue;
	uint8_t powerMode;
	uint8_t gain;
	uint8_t vref;
};

/* Single MCP4728 device data */
struct Mcp4728WholeData {
	uint8_t id;
	uint8_t currI2CAddr;
	uint8_t wantedI2CAddr;
	struct Mcp4728SingleChannelData channelA;
	struct Mcp4728SingleChannelData channelB;
	struct Mcp4728SingleChannelData channelC;
	struct Mcp4728SingleChannelData channelD;
	GPIO_TypeDef * ldacPort;
	uint16_t ldacPin;
	GPIO_TypeDef * readyPort;
	uint16_t readyPin;
	uint8_t internalContent[4][2][3]; // 4 channels, input/eeprom, 3 data bytes
};

extern struct Mcp4728WholeData mcp4728WholeData[MCP4728_AMOUNT];

/* Initialization of MCP4728 devices */
void MCP4728_Init(void);

/* General call commands */
uint8_t MCP4728_Reset(void);
uint8_t MCP4728_WakeUp(void);
uint8_t MCP4728_Update(void);
uint8_t MCP4728_ReadAddress(uint8_t mcp4728Id);

/* Write commands */
uint8_t MCP4728_FastWrite(uint8_t mcp4728Id);
uint8_t MCP4728_MultiWriteAllChannels(uint8_t mcp4728Id);
uint8_t MCP4728_MultiWriteOneChannel(uint8_t mcp4728Id, uint8_t channel);
uint8_t MCP4728_SequentialWrite(uint8_t mcp4728Id, uint8_t startChannel);
uint8_t MCP4728_SingleWrite(uint8_t mcp4728Id, uint8_t channel);
uint8_t MCP4728_WriteAddress(uint8_t mcp4728Id);
uint8_t MCP4728_WriteVref(uint8_t mcp4728Id);
uint8_t MCP4728_WritePowerMode(uint8_t mcp4728Id);
uint8_t MCP4728_WriteGain(uint8_t mcp4728Id);

/* Read commands */
uint8_t MCP4728_ReadConfig(uint8_t mcp4728Id);

/* Getter functions */
uint8_t MCP4728_GetReady(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom);
uint8_t MCP4728_GetPOR(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom);
uint8_t MCP4728_GetVREF(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom);
uint8_t MCP4728_GetPowerMode(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom);
uint8_t MCP4728_GetGain(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom);
uint16_t MCP4728_GetDacValue(uint8_t mcp4728Id, uint8_t channel,
		uint8_t inputRegisterOrEeprom);

#endif /* MCP4728_H_ */
