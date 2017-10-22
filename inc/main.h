/**
 * @author  Szymon Wojtowicz
 * @email   sz_wojtowicz@vp.pl
 * @version V1.0
 * @ide     Keil uVision
 * @brief This file contains the headers of main program.
 */

/**
 * @file
 * @brief Plik naglowkowy
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* amount of ticks to overload SysTick */
#define SYSTICKS 480

/* max length of text */
#define STR_MAX_LENGTH	100

/* pullups */
#define PULLUP1	3300
#define PULLUP2	100
#define PULLUP3	2200

/* current limiting serial resistor */
#define SERIAL1	100
#define SERIAL2	100
#define SERIAL3	100

extern volatile uint32_t uwTimingDelay;
extern volatile uint8_t ms1_flag;
extern volatile uint8_t s1_flag;

void TimingDelay_Decrement(void);
void Delay10Us(volatile uint32_t nTime);
void DelayMs(volatile uint32_t nTime);

#endif /* __MAIN_H */
