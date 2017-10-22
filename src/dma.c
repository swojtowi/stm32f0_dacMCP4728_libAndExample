/*
 * dma.c
 *
 *  Created on: 2 maj 2016
 *      Author: Szymon
 */

#include "dma.h"
#include "main.h"

static uint32_t ledPWMarray[LED_PWM_ARRAY_SIZE];

void DMA_Config(void) {
	DMA_InitTypeDef DMA_InitStructure;
	/*DMA_DeInit(DMA2_Stream0);
	 DMA_DeInit(DMA2_Stream1);*/
	/* DMA2 - channel2, stream1, ADC3 */
	/*DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC3->DR;
	 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) buforADC;
	 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	 DMA_InitStructure.DMA_BufferSize = 2;
	 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	 DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	 DMA_Cmd(DMA2_Stream1, ENABLE);*/
	/* DMA2 - channel0, stream0, ADC1 */
	/*DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) (buforADC + 2);
	 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	 DMA_InitStructure.DMA_BufferSize = 1;
	 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	 DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	 DMA_Cmd(DMA2_Stream0, ENABLE);*/
	/*DMA_DeInit(DMA1_Stream1);
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &TIM2->CCR1;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) ledPWMarray;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = LED_PWM_ARRAY_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream1, ENABLE);*/

	/* DMA2 - channel0, stream0, ADC1 */
	/*DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) (&soundBuf[0][0]);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 16;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	// Enable DMA2 Channel 0 Transfer Complete interrupt
	//DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);*/
}

void LedPWMarray_Init(void) {
	uint32_t i;
	for (i = 0; i < LED_PWM_ARRAY_SIZE / 2; i++) {
		ledPWMarray[i] = i;
	}
	for (i = LED_PWM_ARRAY_SIZE / 2; i < LED_PWM_ARRAY_SIZE; i++) {
		ledPWMarray[i] = 1999 - i;
	}
}
