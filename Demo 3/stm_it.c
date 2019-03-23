/************************************************************
stm_it: Interrupt functions
************************************************************/
#include "Lunar_RBC.h"

#define Rw 0.027
#define Lw 0.1

//PREDEFINED VALUES
volatile int CAR_NO = 1;             //0: BIG X  ; 1: SMALL X
volatile char BALL_COLOR[] = "BBE";  //DEFINE THE COLOR OF THE BALL
volatile char CAR_COLOR[] = "CYW";   //DEFINE THE COLOR OF THE CAR
volatile int MIDLINE_X = 700;        //DEFINE THE LOCATION OF THE MIDDLE LINE
//CAR_NO 0/1 :639 -/+ 50   FOR KICKING TO MIDDLE LINE IS NOT ENOUGH, SO ADD A COMPENSATOR HERE
volatile int ALARM_X = 780;          //DEFINE THE LOCATION OF THE ALARM LINE; WHEN THE BALL REACHES THIS LINE, THE CAR WILL START MOTION 490
volatile int CAR_ORI_X = -1;         //DEFINE THE ORIGINAL POINT(X CORDINATE) OF THE CAR
volatile int CAR_ORI_Y = -1;         //DEFINE THE ORIGINAL POINT(Y CORDINATE) OF THE CAR

int MIN_RESOLUTION = 5;              //MINIMUM RESOLUTION TO CALCULATE DISTANCE
int DEATH = 10;                      //AFTER HOW MANY SAMPLE WILL THE SYSTEM KNOW THE BALL IS DEAD

double Cw = 2*3.1415926535898*Rw;

volatile int L_NORMAL = 700;//1150
volatile int R_NORMAL = 700;
double L_COMPENSATE = 1;

//REFRESHING VALUES
volatile unsigned char receive = '0';
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

volatile int L_C = 0;
volatile int R_C = 0;
volatile double C_A = 0;

bool dead_ball = 0;

volatile bool returned = 0;
//@@@@@@@@@@@@@@@@@@@@
int PID_Val = 0;

/*int record = 0;
char L_buffer[50] = {'\0'};
char R_buffer[50] = {'\0'};
bool L_pulseHigh = false;
bool R_pulseHigh = false;
u32 L_pulseWidth = 0;
u32 R_pulseWidth = 0;
//global
volatile double L_SPEED = 0;
volatile double R_SPEED = 0;
volatile char angle = 0;	*/

void TIM4_IRQHandler(void) //LEFT
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		/*if(L_C>=80000 || R_C>=80000)
		{
      L_C = L_C - 60000;
			R_C = R_C - 60000;
		}	*/	
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_15) == 0) //1
	  {
      L_C--;
    }
	  else
	  {
      L_C++;
    }
    C_A = (R_C-L_C)*Cw/30/Lw;
		if(C_A>=2*PI)
		{
			C_A = C_A-2*PI;
		}
		if(C_A<0)
		{
			C_A = 2*PI+C_A;
		}
		
	  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  }
}

void TIM2_IRQHandler(void) //RIGHT
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		
		/*if(L_C>=80000 || R_C>=80000)
		{
      L_C = L_C - 60000;
			R_C = R_C - 60000;
		}*/		
    if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) == 0) //1 //GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0)
	  {
      R_C--;
    }
	  else
	  {
      R_C++;
    }	
		C_A = (R_C-L_C)*Cw/30/Lw;	
		if(C_A>=2*PI)
		{
			C_A = C_A-2*PI;
		}
		if(C_A<0)
		{
			C_A = 2*PI+C_A;
		}
		
	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}



void TIM1_UP_IRQHandler(void) //SENSOR
{
 
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_TC) != RESET)
	{
		;
	}
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		receive = (unsigned char)USART_ReceiveData(USART2);
		if(receive == '+')
		{
			counter = 0;
		}
		else
		{
			buffer[counter] = receive;
			if(counter == 21)
			{
				if(buffer[7] == CAR_COLOR[0] && buffer[8] == CAR_COLOR[1] && buffer[9] == CAR_COLOR[2]) //CAR INFORMATION
				{
					if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7)) 
	        {
            GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
          }
	        else
	        {
            GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
          } //TESTING CODE TO BLINK LED
					
					//SEE IF THE DISTANCE OF TWO CAR SAMPLE POINT IS FAR ENOUGH TO TAKE THE ANGLE
					//if(calculate_distance(car_last_x_cord, car_last_y_cord, car_x_cord, car_y_cord) > MIN_RESOLUTION)
				  //{
	        //STORE CAR LAST LOCATION
					car_last_x_cord = car_x_cord;
					car_last_y_cord = car_y_cord;
					//}
					
					//REFRESH CAR LOCATION
					car_x_cord = translate_char(buffer[13])*256 + translate_char(buffer[14])*16 + translate_char(buffer[15]);
					car_y_cord = translate_char(buffer[16])*256 + translate_char(buffer[17])*16 + translate_char(buffer[18]);
					
					
					//STORE THE START POINT OF THE CAR
					if(CAR_ORI_X == -1)
					{
						CAR_ORI_X = car_x_cord;
					}
					if(CAR_ORI_Y == -1)
					{
						CAR_ORI_Y = car_y_cord;
					}
				}
				else if(buffer[7] == BALL_COLOR[0] && buffer[8] == BALL_COLOR[1] && buffer[9] == BALL_COLOR[2])
				{
					//STORE BALL LAST LOCATION
					ball_last_x_cord = ball_x_cord;
					ball_last_y_cord = ball_y_cord;
					//REFRESH BALL LOCATION
					ball_x_cord = translate_char(buffer[13])*256 + translate_char(buffer[14])*16 + translate_char(buffer[15]);
					ball_y_cord = translate_char(buffer[16])*256 + translate_char(buffer[17])*16 + translate_char(buffer[18]);
					if(car_y_cord > ball_y_cord)
		      {
		      	ball_y_cord = ball_y_cord - 14;
	      	}
	      	else if(car_y_cord < ball_y_cord)
	      	{
	      		ball_y_cord = ball_y_cord + 14;
	      	}
				}
				
				//########## CALCULATION PART ##########
				//SEE IF THE LOCATION OF POINTS ARE VALID
				if (car_last_x_cord != -1 && car_last_y_cord != -1 && car_x_cord != -1 && car_y_cord != -1 && ball_x_cord != -1 && ball_y_cord != -1)
				{
				  //SEE THE BALL IS IN WHICH REGION
				  if((CAR_NO == 0 && ball_x_cord >= ALARM_X) || (CAR_NO == 1 && ball_x_cord <= ALARM_X)) //BALL REACHES ALARM LINE
					{
						dead_ball = 0;
						returned = 0;
						if(return_flag == 1 && calculate_distance(ball_x_cord,ball_y_cord,ball_last_x_cord,ball_last_y_cord)<= 2 )
						{
						return_flag = 0;
						//MOTION
						PID_Val = (int)(0.5*calculation_PID());
						Start_Move(safe(L_COMPENSATE * (L_NORMAL+PID_Val)),safe(R_NORMAL-PID_Val));
						}
						else if(return_flag == 0)
						{
						//MOTION
						PID_Val = (int)(0.5*calculation_PID());
						Start_Move(safe(L_COMPENSATE * (L_NORMAL+PID_Val)),safe(R_NORMAL-PID_Val));
						}
					}
					else if (return_flag == 0 && ((CAR_NO == 0 && ball_x_cord < ALARM_X && ball_x_cord >= MIDLINE_X)||(CAR_NO == 1 && ball_x_cord > ALARM_X && ball_x_cord <= MIDLINE_X))) //BALL WITHIN DEAD REGION BUT STILL KICKING
					{
						dead_ball = 0;
						return_flag = 0;
						returned = 0;
						//MOTION
						PID_Val = (int)(0.5*calculation_PID());
						Start_Move(safe(L_COMPENSATE * (L_NORMAL+PID_Val)),safe(R_NORMAL-PID_Val));
					}
					else if (return_flag == 1 && dead_ball < DEATH && ((CAR_NO == 0 && ball_x_cord < ALARM_X && ball_x_cord >= MIDLINE_X)||(CAR_NO == 1 && ball_x_cord > ALARM_X && ball_x_cord <= MIDLINE_X))) //BALL WITHIN DEAD REGION BUT NOT DEAD
					{
						dead_ball++;
						return_flag = 1;
						returned = 0;
					}
					else if (dead_ball >= DEATH && ((CAR_NO == 0 && ball_x_cord < ALARM_X && ball_x_cord >= MIDLINE_X)||(CAR_NO == 1 && ball_x_cord > ALARM_X && ball_x_cord <= MIDLINE_X))) //BALL WITHIN DEAD REGION AND DEAD
					{
						dead_ball++;
						return_flag = 0;
						returned = 0;
						//MOTION
						PID_Val = (int)(0.5*calculation_PID());
						Start_Move(safe(L_COMPENSATE * (L_NORMAL+PID_Val)),safe(R_NORMAL-PID_Val));
					}
					else if((CAR_NO == 0 && ball_x_cord < MIDLINE_X) || (CAR_NO == 1 && ball_x_cord > MIDLINE_X)) //BALL OUT OF INTEREST REGION
					{
						dead_ball = 0;
						return_flag = 1;
						//BACK TO START POINT, IF NEAR ENOUGH, STOP AND WAIT
						if((car_x_cord < CAR_ORI_X && CAR_NO == 0)||(car_x_cord > CAR_ORI_X && CAR_NO == 1))//calculate_distance(car_x_cord, car_y_cord, CAR_ORI_X, CAR_ORI_Y) >= 10*MIN_RESOLUTION
						{
							//MOTION
							PID_Val = (int)(0.5*calculation_PID());
							Start_Move(safe(L_COMPENSATE * (L_NORMAL+PID_Val+100)),safe(R_NORMAL-PID_Val+100));
							returned = 0;
						}
						else
						{
							/*if(C_A < PI && C_A > PI/64)
							{
								GPIO_SetBits(GPIOA, GPIO_Pin_0);
                GPIO_ResetBits(GPIOC, GPIO_Pin_15);
	              Start_Move(1000,1000);
								for(int i = 0;i<99999;i++)
								{
									i++;
								}
							}
							else if(C_A >= PI && C_A < 2*PI-PI/64)
							{
								GPIO_ResetBits(GPIOA, GPIO_Pin_0);
                GPIO_SetBits(GPIOC, GPIO_Pin_15);
	              Start_Move(1000,1000);
								for(int i = 0;i<99999;i++)
								{
									i++;
								}
							}*/
							Stop_Move();
							returned = 1;
						}
					}
			  }//CALCULATION PART END
	      
			}
			counter++;
		}
		USART_ClearFlag(USART2, USART_FLAG_RXNE);  
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);  
	}
}







//END HERE
