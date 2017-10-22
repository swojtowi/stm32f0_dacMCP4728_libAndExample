/**
 * @author  Szymon Wojtowicz
 * @email   sz_wojtowicz@vp.pl
 * @version V1.0
 * @ide     Keil uVision
 * @brief This file contains the headers of usart periph.
 */

/**
 * @file
 * @brief Plik naglowkowy
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/**
 * @addtogroup Main_Macros
 * @brief Main defines for PLL, GPIO and others
 * @{
 */

//USART
#define USART1_BAUDRATE 256000
#define USART1_BUFSIZE 128
#define USART1_BUFMASK (USART1_BUFSIZE - 1) //definujemy maske bufora (bufor cykliczny)
#define USART1_TERMINATOR '\r'
#define PRINTF(TEXTBUF)	USART_AddTextToBuf(&USART1_TxBuf, (TEXTBUF))

/**
 * @}
 */

/**
 * @addtogroup Main_Public_Variables
 * @brief Main public variables
 * @{
 */

struct ringBuffer {
	char buf[USART1_BUFSIZE]; // bufor
	uint8_t head; // indeks poczatku bufora
	uint8_t tail; // indeks konca bufora
};

extern struct ringBuffer USART1_TxBuf, USART1_RxBuf;
extern volatile uint8_t usart1RxCmdFlag;

/**
 * @}
 */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
 * @addtogroup Main_Functions
 * @brief 
 * @{
 */

/**
 * @brief USART configuration
 * @retval None
 */
void USART_Config(void);
void USART_AddDataToBuf(struct ringBuffer * usartBuf, char data);
void USART_AddTextToBuf(struct ringBuffer * usartBuf, char * str);
char USART_GetDataFromBuf(struct ringBuffer * usartBuf);
void USART_GetBuf(struct ringBuffer * usartBuf, char * buf, uint8_t len);
//void USART_SendString(const char *str);

/**
 * @}
 */

#endif /* __USART_H */
