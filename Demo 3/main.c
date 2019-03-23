#include "stm32f10x.h" // Device header
#include "stdbool.h"
#include "math.h"

#define PI 3.1415926535

static __IO uint32_t msTicks;
volatile int progress=-1;
volatile int action=0;
volatile char vector[25]="";
volatile int carCoorX=-1;
volatile int carCoorY=-1;
volatile int carCoor1X=-1;
volatile int carCoor1Y=-1;
volatile int ballCoorX=-1;
volatile int ballCoorY=-1;
volatile int car_flag=0;
volatile int ball_flag=0;
volatile char WIFIMessage[]="AT+CIPSTART=\"UDP\",\"0\",0,3105,2\r\n";
volatile char CarColor[]="CVT";
//volatile char CarColor1[]="CRD";
volatile char CarColor1[]="CVT";
volatile char BallColor[]="BRD";
volatile int LeftEdge=500;
volatile int RightEdge=750;
volatile int TurnDegree=1200;
volatile int TurnPower=2000;
volatile int ShootPower=2700;
volatile int ShootTime=2800;
volatile int CommonPower=1500;
volatile int BackTime=1000;
volatile int MPower=17;

volatile int CAR_ORI_X = 0;         //DEFINE THE ORIGINAL POINT(X CORDINATE) OF THE CAR
volatile int CAR_ORI_Y = 0;         //DEFINE THE ORIGINAL POINT(Y CORDINATE) OF THE CAR

//REFRESHING VALUES
volatile unsigned char receive = 0;
volatile int counter = 0;
volatile unsigned char buffer[50];
volatile int car_x_cord = -1;
volatile int car_y_cord = -1;
volatile int ball_x_cord = -1;
volatile int ball_y_cord = -1;

volatile int car_last_x_cord = -1;
volatile int car_last_y_cord = -1;
volatile int ball_last_x_cord = -1;
volatile int ball_last_y_cord = -1;
volatile int return_flag = 0;

volatile double C_A = 0;
volatile bool returned = 0;
//@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@CARDINATE

double car_angle = 0;        //POSITIVE: 2/4  ;  NEGATIVE: 1/3
double ball_angle = 0;       //POSITIVE: 2/4  ;  NEGATIVE: 1/3
int car_dir = 0;             //0:FORWARD  ;  1:BACKWARD
int ball_dir = 0;            //0:FORWARD  ;  1:BACKWARD
double relative_angle = 0;   //RELATIVE ANGLE BETWEEN CAR DIRECTION AND BALL LOCATION; 
double Kp = 600; //1500
double Ki = 0;
double Kd = 0;
double now_error = 0;
double now_output = 0;
double past_error = 0;
double Ts = 0;
int PID_Val = 0;

void DelayMs(uint32_t ms);
void GPIOInit(){
	//GPIO Init PA0, BPHASE
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO Init PC15, APHASE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO Init PB7, LED
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO Init PB8, Switch
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// Configure the SysTick timer to overflow every 1 ms
	SysTick_Config(SystemCoreClock / 1000);
}
void WheelInit(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure I/O for Tim3 Ch1 PWM pin
	// TIM3 CH1 PWM  **  PA6      ** D12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Configure I/O for Tim3 Ch2 PWM pin
	// TIM3 CH2 PWM  **  PA7      ** D11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 144-1;  //1/(72Mhz/1440)=0.2ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 5000-1;  
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &timerInitStructure);
  TIM_Cmd(TIM3, ENABLE);
	
	TIM_OCInitTypeDef outputChannelInit;
	//Enable Tim3 Ch1 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//Enable Tim3 Ch2 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
}
void USARTInit(){
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
	//USART_ClockInitTypeDef USART_ClockInitStructure; 
	
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
		
	NVIC_InitTypeDef NVIC_InitStructure;
	// Enable the USART2 TX Interrupt 
	USART_ITConfig(USART2, USART_IT_TC, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// Enable the USART2 RX Interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void USARTInitNoInterrupt(){
	//USART2 TX RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART2 ST-LINK USB
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStructure; 
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}
void USARTSend(volatile char arr[]){
	int count=0;
	for(;arr[count]!='\0';count++)
	{
		
	}
	for(int i=0;i<count;i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, arr[i]); 
	}
}
void TIM2_init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 18000; //1/(72Mhz/18000)=0.25ms
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 200; //0.25ms*200 = 50ms
	timerInitStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	//Enable update event and interrupt for Timer2
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
}
int convert(char c) {
	switch(c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
	}
	return 0;
}
void parseCarCoor(){
	carCoorX= convert(vector[6])*256 + convert(vector[7])*16 + convert(vector[8]);
	carCoorY= convert(vector[9])*256 + convert(vector[10])*16 + convert(vector[11]);
	car_flag=(car_flag+1)%5;
}
void parseCarCoor1(){
	carCoor1X= convert(vector[6])*256 + convert(vector[7])*16 + convert(vector[8]);
	carCoor1Y= convert(vector[9])*256 + convert(vector[10])*16 + convert(vector[11]);
	car_flag=(car_flag+1)%5;
}
void parseBallCoor(){
	ballCoorX= convert(vector[6])*256 + convert(vector[7])*16 + convert(vector[8]);
	ballCoorY= convert(vector[9])*256 + convert(vector[10])*16 + convert(vector[11]);
	ball_flag=(ball_flag+1)%5;
}
void LEDLight(){
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7)) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
		} else {
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
		}
}
void LEDOn(){
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
}
void LEDOff(){
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
}
void DelayMs(uint32_t ms){
	// Reload us value
	msTicks = ms;
	// Wait until usTick reach zero
	while (msTicks);
}
void SysTick_Handler(){
	if (msTicks != 0)
	{
		msTicks--;
	}
}
void relative_location(int x1, int y1, int x2, int y2);
double calculate_angle(int x1, int y1, int x2, int y2);
double calculate_distance(int x1, int y1, int x2, int y2);
int abs_val(int input);
double calculation_PID(){
	double result;
	if(return_flag == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);//Reset
	  GPIO_SetBits(GPIOC, GPIO_Pin_15);//Reset
		relative_location(car_x_cord, car_y_cord, ball_x_cord, ball_y_cord);
		relative_angle = -relative_angle;
		if(calculate_distance(car_x_cord, car_y_cord, ball_x_cord, ball_y_cord) > 60)
		{
			Kp = 1000;//2650
		}
		else if(calculate_distance(car_x_cord, car_y_cord, ball_x_cord, ball_y_cord) > 35)
		{
			Kp = 350;  //870//1537
		}
		else
		{
			Kp = 100;//725
		}
	}
	else if(return_flag == 1)
	{
		if(car_x_cord > CAR_ORI_X)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);//Set
	    GPIO_ResetBits(GPIOC, GPIO_Pin_15);//Set
			relative_location(car_x_cord, car_y_cord, CAR_ORI_X, CAR_ORI_Y);
			relative_angle = -relative_angle;
			if(calculate_distance(car_x_cord, car_y_cord, CAR_ORI_X, CAR_ORI_Y) > 120 && abs_val(relative_angle) > 0.0872665)
		  {
		  	Kp = 130;//265
		  }
	  	else
	  	{
	  		Kp = 0;
		  }
		}
		else
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_0);//Reset
	    GPIO_SetBits(GPIOC, GPIO_Pin_15);//Reset
		  relative_location(car_x_cord, car_y_cord, CAR_ORI_X, CAR_ORI_Y);
			relative_angle = -relative_angle;
		}
	}
	
	now_error = relative_angle;
	
	now_output = (Kp + Ki * Ts) * now_error + Ki * past_error;
	
	result = now_output; 
	return result;
}
void relative_location(int x1, int y1, int x2, int y2){
	//CALCULATE THE RELATIVE DIRECTION
	
		ball_angle = calculate_angle(x1,y1,x2,y2);
	
	  car_angle = C_A;
		relative_angle = ball_angle - car_angle;
	  if(relative_angle > PI)
		{
			relative_angle = relative_angle - 2*PI;
		}
		if(relative_angle < -PI)
		{
			relative_angle = relative_angle + 2*PI;
		}
}
int safe(int value){
	if(value>1200)//1800)
	{
		value = 1200; //1800
	}
	if (value<0)
	{
		value = 0;
	}
	return value;
}
double calculate_angle(int x1, int y1, int x2, int y2){
	double result = 1.0*atan2(abs_val(y1-y2),abs_val(x1-x2));
		if(x1>=x2)
	  {
		  if(y1<y2)
			{
				return result;
			}
			else
			{
				return (2*PI-result);
			}
		}
		else if(x1<x2)
		{
			if(y1<y2)
			{
				return (PI-result);
			}
			else
			{
				return (PI+result);
			}
		}
		return 0;
/*
		if(x1<=x2)
	  {
		  if(y1>y2)
			{
				return result;
			}
			else
			{
				return (2*PI-result);
			}
		}
		else if(x1>x2)
		{
			if(y1>y2)
			{
				return (PI-result);
			}
			else
			{
				return (PI+result);
			}
		}
		return 0;
		//Configuration for car 2;
		*/
}
double calculate_distance(int x1, int y1, int x2, int y2){
	return (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}
int abs_val(int input){
	if(input<0)
	{
		return (-input);
	}
	else
	{
	  return input;	
	}
}
void LeftWheel(int cycle){
	if(cycle<0)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		TIM_OCInitTypeDef outputChannelInit;
		outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
		outputChannelInit.TIM_Pulse = -1*cycle; 
		outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
		outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC2Init(TIM3, &outputChannelInit);
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
	else if(cycle>=0)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		TIM_OCInitTypeDef outputChannelInit;
		outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
		outputChannelInit.TIM_Pulse = cycle; 
		outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
		outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC2Init(TIM3, &outputChannelInit);
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
}
void RightWheel(int cycle){
	if(cycle<0)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);
		TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = -1*cycle; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
	else if(cycle>=0)
	{
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_RESET);
		TIM_OCInitTypeDef outputChannelInit;
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = cycle; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
}
void Stop(int period){
	LeftWheel(0);
	RightWheel(0);
	DelayMs(period);
}
void Move(int speed,int period){
	PID_Val = (int)(0.5*calculation_PID());
	LeftWheel(safe(speed+PID_Val));
	RightWheel(safe(speed-PID_Val));
	DelayMs(period);
	Stop(100);
}
void TurnRight90(){
	LeftWheel(TurnPower);
	RightWheel(-TurnPower);
	DelayMs(TurnDegree);
	Stop(500);
}
void TurnRight90Back(){
	LeftWheel(-TurnPower);
	RightWheel(TurnPower);
	DelayMs(TurnDegree);
	Stop(500);
}
void turnright(int cycle){
	LeftWheel(cycle);
	RightWheel(-cycle);
}
void turnleft(int cycle){
	LeftWheel(-cycle);
	RightWheel(cycle);
}
void Procedure1(){
	LEDOn();
	Move(CommonPower,MPower*(carCoorY-ballCoorY));
	TurnRight90();
	//Stop(1000);
	//Move(-CommonPower,BackTime);//Back to bottom
	//Stop(2000);
	for(int i=0;i<5;i++){
		turnright(10*i);
		Move(ShootPower,ShootTime);
		Move(-ShootPower,ShootTime);
	}
	//Move(ShootPower,ShootTime);//Hit power and car stop position
	Stop(1000);
}
void Procedure2(){
	Move(-CommonPower,1500);
	TurnRight90Back();
	Stop(1000);
	Move(-CommonPower,3000);//Back to bottom
	Stop(3000);
}
int main(void) {
	GPIOInit();
	WheelInit();
	TIM2_init();
	USARTInitNoInterrupt();
	//DelayMs(2000);
	USARTSend(WIFIMessage);//Wifi connection
	DelayMs(2000);
	USARTInit();
	LEDOn();
	DelayMs(8000);
	Procedure1();
	DelayMs(2000);
	Procedure2();
	while(1){
			while(ballCoorX!= -1){
			while(carCoorX!= -1){
					LEDOn();
					if(ballCoorX<LeftEdge){
						Procedure1();
					}
					else {
						Procedure2();
					}
				
			}
	}
	}
}
//Serial Port Interrupt Handler
void USART2_IRQHandler() {
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	//LEDOn();
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		//LEDOn();
		char data = USART_ReceiveData(USART2);
		if( data==':')
		{
			progress=0;
			return;
		}
		else if( data=='\n')
		{
			progress=-1;
			//******************
			if(vector[0]==CarColor[0] && vector[1]==CarColor[1] && vector[2]==CarColor[2])
			{
				parseCarCoor();
				action=1;
			}
			else if(vector[0]==BallColor[0] && vector[1]==BallColor[1] && vector[2]==BallColor[2])
			{
				parseBallCoor();
				action=1;
			}
			else if(vector[0]==CarColor1[0] && vector[1]==CarColor1[1] && vector[2]==CarColor1[2])
			{
				parseCarCoor1();
				action=1;
			}
			//******************
			return;
		}
		else if(progress!=-1)
		{
			vector[progress]=data;
			progress++;
		}	
		car_last_x_cord = carCoorX;
		car_last_y_cord = carCoorY;
		ball_last_x_cord = ballCoorX;
		ball_last_y_cord = ballCoorY;
		DelayMs(100);
		if( data==':')
		{
			progress=0;
			return;
		}
		else if( data=='\n')
		{
			progress=-1;
			//******************
			if(vector[0]==CarColor[0] && vector[1]==CarColor[1] && vector[2]==CarColor[2])
			{
				parseCarCoor();
				action=1;
			}
			else if(vector[0]==BallColor[0] && vector[1]==BallColor[1] && vector[2]==BallColor[2])
			{
				parseBallCoor();
				action=1;
			}
			else if(vector[0]==CarColor1[0] && vector[1]==CarColor1[1] && vector[2]==CarColor1[2])
			{
				parseCarCoor1();
				action=1;
			}
			//******************
			return;
		}
		else if(progress!=-1)
		{
			vector[progress]=data;
			progress++;
		}	
		car_x_cord = carCoorX;
		car_y_cord = carCoorY;
		ball_x_cord = ballCoorX;
		ball_y_cord = ballCoorY;
	}	
}
void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		LEDLight();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}                                    
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
