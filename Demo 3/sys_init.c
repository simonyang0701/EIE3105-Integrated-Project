/************************************************************
sys_init: System initialize functions
************************************************************/
#include "Lunar_RBC.h"

volatile uint32_t msTicks = 0;
volatile int L_DUTY = 1;  
volatile int R_DUTY = 1;                                                                                                                                                                                                     ;  

void Init_Sys(void)
{
	Init_RCC();
	Init_GPIO();
  Init_TIM3_PWM();
  Init_TIM2_4_CAPT();
  Init_USART();
	Sys_DelayClock_Init();
	//Init_TIM1();
}

void DelayMs(uint32_t ms)
{
    msTicks = ms;
    while(msTicks);
}


//####
void Init_RCC(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}

void Init_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//LEFT PHASE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//RIGHT PHASE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//BUTTON
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//PORTB.8 AS INPUT
	
	NVIC_InitTypeDef NVIC_Stru;
	NVIC_Stru.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Stru.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Stru.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&NVIC_Stru);
}

void Init_TIM3_PWM(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//TIM3_CH1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//TIM3_CH2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//TIM3 SET UP 
  TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 720-1;  //1/(72Mhz/720)=0.01ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 2000-1;    //0.01ms * 2000 = 20ms -> 50Hz  ; 2000counts -> 100% duty
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &timerInitStructure);
  TIM_Cmd(TIM3, ENABLE);

	TIM_OCInitTypeDef outputChannelInit;
	//Enable Tim3 Ch1 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = L_DUTY-1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
  //Enable Tim3 Ch2 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = R_DUTY-1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
}

void Init_TIM2_4_CAPT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//TIM4_CH1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
	//TIM2_CH2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_Struct;
	//TIM4
	TIM_Struct.TIM_Prescaler = 0;   //1/(72Mhz/72)=1us
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Struct.TIM_Period = 1;          
	TIM_Struct.TIM_ClockDivision = 0;
	TIM_Struct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_Struct); //TIM2:1S*/
	TIM_Cmd(TIM4, ENABLE);
	
	//TIM2
	TIM_Struct.TIM_Prescaler = 0;   //1/(72Mhz/72)=1us
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Struct.TIM_Period = 1;         
	TIM_Struct.TIM_ClockDivision = 0;
	TIM_Struct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_Struct); //TIM4:1S*/
  TIM_Cmd(TIM2, ENABLE);
	
	//TIM2 EXTERNAL INTERRUPT
	TIM_TIxExternalClockConfig(TIM2, TIM_TIxExternalCLK1Source_TI2, TIM_ICPolarity_Rising, 0); //PORTA.1 AS INPUT
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
	//TIM4 EXTERNAL INTERRUPT
	TIM_TIxExternalClockConfig(TIM4, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0); //PORTB.6 AS INPUT
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM4_IRQn);
}

void Init_USART(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//USART TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART INITIALIZE
	USART_InitTypeDef USART_InitS;
	USART_InitS.USART_BaudRate = 115200;
	USART_InitS.USART_WordLength = USART_WordLength_8b;
	USART_InitS.USART_StopBits = USART_StopBits_1;
	USART_InitS.USART_Parity = USART_Parity_No;
	USART_InitS.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitS.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitS);
	USART_Cmd(USART2, ENABLE);
	
	//USART INTERRUPT ENABLE
	NVIC_InitTypeDef NVIC_InitS;
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	//USART_ITConfig(USART2,USART_IT_TC,ENABLE);
	NVIC_InitS.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitS.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitS.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitS.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitS);

}


void Init_TIM1(void)
{
  TIM_TimeBaseInitTypeDef timerInitStructure;
	
  timerInitStructure.TIM_Prescaler = 18000-1; //1/(72Mhz/18000)=0.25ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 10-1; //0.25ms*200 = 50ms
  timerInitStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &timerInitStructure);
  TIM_Cmd(TIM1, ENABLE);
  //Enable update event and interrupt for Timer1
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void SysTick_Handler()
{
    if (msTicks != 0)
    {
        msTicks --;
    }
}

void Sys_DelayClock_Init(void)
{
	SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000); //config the SysTick timer to overflow evey 1ms
}





//END HERE
