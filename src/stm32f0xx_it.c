/**
 * @file
 * @author  MCD Application Team & Szymon Wojtowicz
 * @email sz_wojtowicz@vp.pl
 * @ide     Keil uVision
 * @version V1.0.0
 * @date    20-June-2015
 * @brief   This source file contains definitions of the interrupt handlers.
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "dma.h"
#include "stm32f0xx_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @retval None
 */
void NMI_Handler(void) {
}

/**
 * @brief  This function handles Hard Fault exception.
 * @retval None
 */
void HardFault_Handler(void) {
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @retval None
 */
void MemManage_Handler(void) {
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @retval None
 */
void BusFault_Handler(void) {
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @retval None
 */
void UsageFault_Handler(void) {
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @retval None
 */
void SVC_Handler(void) {
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @retval None
 */
void DebugMon_Handler(void) {
}

/**
 * @brief  This function handles PendSVC exception.
 * @retval None
 */
void PendSV_Handler(void) {
}

void EXTI4_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
		//USART_AddTextToBuf(&USART1_TxBuf, "Jestem w przerwaniu od RFM75\n");
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}

/**
 * @brief  This function handles EXTI0 interrupt.
 * @retval None
 */
void EXTI15_10_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
		USART_AddTextToBuf(&USART1_TxBuf, "Jestem w przerwaniu od buttona\n");
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

/**
 * @brief  This function handles RTC WakeUp interrupt.
 * @retval None
 */
void RTC_WKUP_IRQHandler(void) {
}

void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_AddDataToBuf(&USART1_RxBuf, (char) USART1->RDR);
		if((char)(USART1->RDR) == USART1_TERMINATOR) {
			usart1RxCmdFlag = 1;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
		char c = USART_GetDataFromBuf(&USART1_TxBuf);
		if (c) {
			USART1->TDR = (uint16_t) c;
		} else {
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
}

/*void DMA2_Stream0_IRQHandler(void) {
 //Test on DMA2 Stream 0 Transfer Complete interrupt
 if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET) {
 //USART_AddTextToBuf(&USART1_TxBuf, "Jestem w przerwaniu od DMA\n");
 soundBufNumberOwn ^= 1;
 DMA_SOUNDTX_STOP;
 DMA2_Stream0->M0AR = (uint32_t) (&soundBufOwn[soundBufNumberOwn][0]);
 //DMA2_Stream0->M0AR = (uint32_t) (&soundBufReceived[soundBufNumberReceived^1][0]);
 DMA_SOUNDTX_START;
 soundSendIrqFlag = 1;
 //licznikADC++;
 //Clear DMA2 Stream0 Half Transfer, Transfer Complete and Global interrupt pending bits
 DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
 }
 }*/

/*void DMA1_Stream5_IRQHandler(void) {
 //Test on DMA1 Stream 5 Transfer Complete interrupt
 if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5) != RESET) {
 //USART_AddTextToBuf(&USART1_TxBuf, "Jestem w przerwaniu od DMA\n");
 soundBufNumberReceived ^= 1;
 DMA_SOUNDDAC_STOP;
 DMA1_Stream5->M0AR =
 (uint32_t) (&soundBufReceived[soundBufNumberReceived][0]);*/
/*DMA1_Stream5->M0AR =
 (uint32_t) (&soundBufOwn[soundBufNumberOwn ^ 1][0]);*/
/*DMA_SOUNDDAC_START;
 //Clear DMA1 Stream5 Half Transfer, Transfer Complete and Global interrupt pending bits
 DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
 }
 }*/

void ADC_IRQHandler(void) {
	//Test on ADC EndOfConversion interrupt
	if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
		USART_AddTextToBuf(&USART1_TxBuf, "Jestem w przerwaniu od ADC\n");
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
	}
}

void TIM2_IRQHandler(void) {
	/*static uint16_t ch1;
	 static uint8_t direction;*/
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		/*if (direction == 0) {
		 if (ch1++ > 999) {
		 direction = 1;
		 }
		 }
		 if (direction == 1) {
		 if (ch1-- < 2) {
		 direction = 0;
		 }
		 }
		 TIM2->CCR1 = ch1;*/
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

/**
 * @brief  This function handles SysTick Handler.
 * @retval None
 */
void SysTick_Handler(void) {
	static uint16_t us10_cnt = 0;
	static uint16_t ms1_cnt = 0;

	if (++us10_cnt > 99) {
		ms1_flag = 1;
		us10_cnt = 0;
		if (++ms1_cnt > 999) {
			s1_flag = 1;
			ms1_cnt = 0;
		}
	}
	TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @retval None
 */
/*void PPP_IRQHandler(void)
 {
 }*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
