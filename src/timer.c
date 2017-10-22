/*
 * timer.c
 *
 *  Created on: 2 maj 2016
 *      Author: Szymon
 */

#include "timer.h"

/**
 * @brief TIM configuration
 * @retval None
 */
void TIM_Config(void) {
	/* Timer2 trigerring ADC conversions */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	/*TIM_TimeBaseStructure.TIM_Period = 50000;
	 TIM_TimeBaseStructure.TIM_Prescaler = 449;
	 TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	 TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	 TIM_Cmd(TIM2, ENABLE);*/

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 44;  //fclk = 72M/72M - 1 = 0
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //0 = nie dzielimy zegara
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //tryb zliczania w górê
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	// Konfiguracja kanalu 1 ktory jest wyprowadzony na PA5
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Sygna³ z timera bêdzie wykorzystywany do sterowania kontrolerem przerwañ wiêc musi byæ Enable
	TIM_OCInitStructure.TIM_Pulse = 0; //100% PWM
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //stan wysoki
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	/*TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	 TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);*/
	TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);
	// Wlaczenie timera
	TIM_Cmd(TIM2, ENABLE);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 1124;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
	TIM_Cmd(TIM3, ENABLE);
}
