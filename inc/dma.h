/*
 * dma.h
 *
 *  Created on: 2 maj 2016
 *      Author: Szymon
 */

#ifndef DMA_H_
#define DMA_H_

#include "stm32f0xx.h"

#define LED_PWM_ARRAY_SIZE 2000

/**
 * @brief DMA configuration
 * @retval None
 **/
void DMA_Config(void);
void LedPWMarray_Init(void);

#endif /* DMA_H_ */
