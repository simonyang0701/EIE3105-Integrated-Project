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

static __IO uint8_t state = 1;
static __IO uint8_t sec = 0;
static uint8_t count = 0;

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
		//GPIO_WriteBit(GPIOA, L1_G_Pin, !((BitAction)GPIO_ReadOutputDataBit(GPIOA, L1_G_Pin)));
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

int main() {
	init();
	
	SystemCoreClockUpdate();
	
	SysTick_Config(SystemCoreClock/10); //set sysTick to 100ms
	
	state1();
	
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


