/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include <stdlib.h>

struct ringBuffer USART1_TxBuf, USART1_RxBuf;
volatile uint8_t usart1RxCmdFlag;

void USART_Config(void) {
	USART_InitTypeDef USART_InitStructure;

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART1, &USART_InitStructure);

	/* Here the USART1 receive interrupt is enabled
	 * and the interrupt controller is configured 
	 * to jump to the USART1_IRQHandler() function
	 * if the USART1 receive interrupt occurs
	 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt
	USART_Cmd(USART1, ENABLE);
}

void USART_AddDataToBuf(struct ringBuffer * usartBuf, char data) // funkcja wysylajaca 1 bajt
{

	usartBuf->head = ((usartBuf->head + 1) & USART1_BUFMASK); // ustawianie kolejnych pozycji w buforze cyklicznym

	if (usartBuf->head == usartBuf->tail) {
		return;
	}
	usartBuf->buf[usartBuf->head] = data; // wpisanie danych do buforu

	if (usartBuf == &(USART1_TxBuf)) {
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); // enable the USART1 transmit buffer empty interrupt
	}
}

void USART_AddTextToBuf(struct ringBuffer * usartBuf, char * str) {
	register char c = *str; // deklaracja zmiennej na potrzeby funkcji
	while (c) {
		USART_AddDataToBuf(usartBuf, c);
		c = *(++str);
		// petla wysylajaca kolejne znaki
	}
}

char USART_GetDataFromBuf(struct ringBuffer * usartBuf) {
	if (usartBuf->tail == usartBuf->head) {
		return 0; // jesli dane nie przyszly zwaracamy zero
	}
	usartBuf->tail = ((usartBuf->tail + 1) & USART1_BUFMASK); // ustalamy nowy adres konca buforu

	return usartBuf->buf[usartBuf->tail]; // zwracamy wynik w postaci wartosci danego pola w buforze

}

void USART_GetBuf(struct ringBuffer * usartBuf, char * buf, uint8_t len) // funkcja do odczytu buforu
{
	while (len--) {
		*buf++ = USART_GetDataFromBuf(usartBuf); // odczytujemy dane dopóki len > 0
	}
}

/*void USART_SendString(const char *str) {
	while (*str) {
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USART1, *str++);
	}
}*/
