#include "stm32f10x.h" // Device header
#include "string.h"

unsigned char character;
static unsigned char state = 0; //no stored char

int main(void) {
	//USART2 TX RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART2 ST-LINK USB
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	// Enable the USART2 TX Interrupt
	USART_ITConfig(USART2, USART_IT_TC, ENABLE); //transmission complete
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the USART2 RX Interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //receive data register not empty
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	while(1) {
	}
}
void USART2_IRQHandler() {
	if (state == 1) { //have stored data
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) { //rxed data
			if (character == (unsigned char) USART_ReceiveData(USART2)) {
				state = 0;
			}
		}
	} else { //no stored data
		//check if character received
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) { //rxed data
			character = (unsigned char) USART_ReceiveData(USART2);
			for (unsigned char i=0; i<10; i++) {
				while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
				USART_SendData(USART2, character);
			}
			state = 1;
		}
		//send aaa
		else if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) { //transmission completed
			USART_SendData(USART2, 'a');
			USART_ClearITPendingBit(USART2, USART_IT_TC);
		}
	}
}
