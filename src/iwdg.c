/*
 * iwdg.c
 *
 *  Created on: 12 cze 2016
 *      Author: Szymon
 */

#include "iwdg.h"

/**
 * @brief Independent Watchdog configuration
 */
void IWDG_Config(void) {
	/* IWDG timeout equal to 1500 ms (the timeout may varies due to LSI frequency
	 dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: LSI/32 */
	IWDG_SetPrescaler(IWDG_Prescaler_32);

	/* Set counter reload value to obtain 250ms IWDG TimeOut.
	 IWDG counter clock Frequency = LsiFreq/32
	 Counter Reload Value = 1520ms/IWDG counter clock period
	 = 1.52s / (32/LsiFreq)
	 = LsiFreq/(32 * 100 / 152)
	 = LsiFreq/21
	 */
	IWDG_SetReload(32000 / 10);

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}
