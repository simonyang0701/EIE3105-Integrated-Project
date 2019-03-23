#include "stm32f10x.h" // Device header
#include "stdbool.h" // For Boolean data type

//Button: PB6 Tim4 Ch1
#define BUTTON_RCC_GPIO RCC_APB2Periph_GPIOB
#define BUTTON_GPIO GPIOB
#define BUTTON_GPIO_PIN GPIO_Pin_6
#define BUTTON_RCC_TIM RCC_APB1Periph_TIM4
#define BUTTON_TIM TIM4

//LED: PA5
#define LED_RCC_GPIO RCC_APB2Periph_GPIOA
#define LED_GPIO GPIOA
#define LED_GPIO_PIN GPIO_Pin_5

int main(void) {
	// GPIO clock for LED and button
	RCC_APB2PeriphClockCmd(BUTTON_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(LED_RCC_GPIO, ENABLE);
	RCC_APB1PeriphClockCmd(BUTTON_RCC_TIM, ENABLE);
	
	// Configure I/O for LED
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_GPIO, &GPIO_InitStruct);
	
	//Timer 4 set up
	GPIO_InitStruct.GPIO_Pin = BUTTON_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(BUTTON_GPIO, &GPIO_InitStruct);
	
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 0;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 2;
	timerInitStructure.TIM_ClockDivision = 0;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(BUTTON_TIM, &timerInitStructure);
	TIM_Cmd(BUTTON_TIM, ENABLE);
	TIM_TIxExternalClockConfig(BUTTON_TIM, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
	
	//Enable update event for Timer4
	TIM_ITConfig(BUTTON_TIM, TIM_IT_Update, ENABLE);
	//Enable interrupt
	NVIC_EnableIRQ(TIM4_IRQn);
	
	while(1) {}
}

void TIM4_IRQHandler(void) {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) { //if int occurs
		//Toggle LED
		LED_GPIO->ODR ^= LED_GPIO_PIN;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
