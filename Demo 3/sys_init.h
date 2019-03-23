/************************************************************
sys_init: System initialize functions
************************************************************/


void Init_Sys(void);

//####
void Init_RCC(void);
void Init_GPIO(void);
void Init_TIM3_PWM(void);
void Init_TIM2_4_CAPT(void);
void Init_USART(void);
void Init_TIM1(void);

void Sys_DelayClock_Init(void);
void DelayMs(uint32_t ms);

//END HERE
