#include "stm32f10x.h"                  // Device header

//L3_R=PA0, L3_Y=PA1, L3_G=PA4, L2_R=PA5, L2_Y=PA6, L2_G=PA7, L1_R=PA8, L1_G=PA9

#define LED_RCC_GPIO RCC_APB2Periph_GPIOA
#define L1_R_Pin GPIO_Pin_8
#define L1_G_Pin GPIO_Pin_9
#define L2_G_Pin GPIO_Pin_7
#define L2_Y_Pin GPIO_Pin_6
#define L2_R_Pin GPIO_Pin_5
#define L3_G_Pin GPIO_Pin_4
#define L3_Y_Pin GPIO_Pin_1
#define L3_R_Pin GPIO_Pin_0

//PC13
#define BUTTON_RCC_GPIO RCC_APB2Periph_GPIOC
#define BUTTON_GPIO GPIOC
#define BUTTON_GPIO_PIN GPIO_Pin_13
#define BUTTON_EXTI_LINE EXTI_Line13
#define BUTTON_GPIO_PORTSOURCE GPIO_PortSourceGPIOC
#define BUTTON_GPIO_PINSOURCE GPIO_PinSource13

static __IO uint8_t state = 1;
static __IO uint8_t sec = 0;
static uint8_t count = 0;
static __IO uint8_t enable = 0;

void init() {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(LED_RCC_GPIO, ENABLE);
	
	//init all LEDSs
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStruct.GPIO_Pin = L1_R_Pin | L1_G_Pin | L2_G_Pin | L3_R_Pin | L2_Y_Pin | L2_R_Pin | L3_G_Pin | L3_Y_Pin;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void state1(void);
void state2(void);
void state3(void);
void state4(void);
void state5(void);
void state6(void);
void state7(void);
void state8(void);

void SysTick_Handler()
{
	count++;
	if (state == 6 && count != 10)
	{
		GPIOA->ODR ^= L1_G_Pin;
	}
	
	if (count == 10)
	{
		count = 0;
		switch (state)
		{
			case 1:
				sec++;
				if (sec == 5) {
					state2();
					sec = 0;
				}
				break;
			case 2:
				state3();
				break;
			case 3:
				state4();
				break;
			case 4:
				state5();
				break;
			case 5:
				sec++;
				if (sec == 5) {
					state6();
					sec = 0;
				}
				break;
			case 6:
				state7();
				break;
			case 7:
				state8();
				break;
			case 8:
				state1();
				break;
		}
	}
}


void exti_init(void) {
	RCC_APB2PeriphClockCmd(BUTTON_RCC_GPIO, ENABLE); // GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //AFIO
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Configure I/O for EXTI13
	GPIO_InitStruct.GPIO_Pin = BUTTON_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BUTTON_GPIO, &GPIO_InitStruct);
	
	// EXTI Configuration, GPIOC, Pin 13
	GPIO_EXTILineConfig(BUTTON_GPIO_PORTSOURCE, BUTTON_GPIO_PINSOURCE);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = BUTTON_EXTI_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// Enable Interrupt
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
}

int main() {
	init();
	exti_init();
	
	SystemCoreClockUpdate();
	
	SysTick_Config(SystemCoreClock/10); //set sysTick to 100ms
	
	//state1();
	
	SysTick->CTRL &= ~(0x00000001); //disable SysTick
	
	//GPIO_SetBits(GPIOA, L1_R_Pin | L1_G_Pin | L2_G_Pin | L3_R_Pin | L2_Y_Pin | L2_R_Pin | L3_G_Pin | L3_Y_Pin);
	while(1) {
	}
}

void state1() {
	state = 1;
	GPIO_SetBits(GPIOA, L1_R_Pin | L2_G_Pin | L3_R_Pin);
	GPIO_ResetBits(GPIOA, L1_G_Pin | L2_Y_Pin | L2_R_Pin | L3_G_Pin | L3_Y_Pin);
}

void state2() {
	state = 2;
	GPIO_SetBits(GPIOA, L1_R_Pin | L2_Y_Pin | L3_R_Pin);
	GPIO_ResetBits(GPIOA, L1_G_Pin | L2_G_Pin | L2_R_Pin | L3_G_Pin | L3_Y_Pin);
}

void state3() {
	state = 3;
	GPIO_SetBits(GPIOA, L1_R_Pin | L2_R_Pin | L3_R_Pin);
	GPIO_ResetBits(GPIOA, L1_G_Pin | L2_G_Pin | L2_Y_Pin | L3_G_Pin | L3_Y_Pin);
}

void state4() {
	state = 4;
	GPIO_SetBits(GPIOA, L1_R_Pin | L2_R_Pin | L3_R_Pin | L3_Y_Pin);
	GPIO_ResetBits(GPIOA, L1_G_Pin | L2_G_Pin | L2_Y_Pin | L3_G_Pin);
}

void state5() {
	state = 5;
	GPIO_SetBits(GPIOA, L1_G_Pin | L2_R_Pin | L3_G_Pin);
	GPIO_ResetBits(GPIOA, L1_R_Pin | L2_G_Pin | L2_Y_Pin | L3_R_Pin | L3_Y_Pin);
}

void state6() {
	state = 6;
	GPIO_SetBits(GPIOA, L1_G_Pin | L2_R_Pin | L3_Y_Pin);
	GPIO_ResetBits(GPIOA, L1_R_Pin | L2_G_Pin | L2_Y_Pin | L3_R_Pin | L3_G_Pin);
}

void state7() {
	state = 7;
	GPIO_SetBits(GPIOA, L1_R_Pin | L2_R_Pin | L3_R_Pin);
	GPIO_ResetBits(GPIOA, L1_G_Pin | L2_G_Pin | L2_Y_Pin | L3_G_Pin | L3_Y_Pin);
}

void state8() {
	state = 8;
	GPIO_SetBits(GPIOA, L1_R_Pin | L2_R_Pin | L2_Y_Pin | L3_R_Pin);
	GPIO_ResetBits(GPIOA, L1_G_Pin | L2_G_Pin | L3_G_Pin | L3_Y_Pin);
}

void EXTI15_10_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
		if (enable) {
			SysTick->CTRL &= ~(0x00000001); //disable SysTick
			GPIO_ResetBits(GPIOA, L1_R_Pin | L1_G_Pin | L2_G_Pin | L3_R_Pin | L2_Y_Pin | L2_R_Pin | L3_G_Pin | L3_Y_Pin);
		} else {
			switch (state)
			{
				case 1:
					state1();
					break;
				case 2:
					state2();
					break;
				case 3:
					state3();
					break;
				case 4:
					state4();
					break;
				case 5:
					state5();
					break;
				case 6:
					state6();
					break;
				case 7:
					state7();
					break;
				case 8:
					state8();
					break;
			}
			SysTick->CTRL |= 0x00000001; //enable SysTick
		}
	enable = 1-enable;
	EXTI_ClearITPendingBit(EXTI_Line13);
	}
}
