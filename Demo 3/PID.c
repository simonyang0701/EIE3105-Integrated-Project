/************************************************************

************************************************************/
#include "Lunar_RBC.h"

//@@@@@@@@@@@@@@@@@@@@WIFI

//PREDEFINED VALUES
extern volatile int CAR_NO;
extern volatile char BALL_COLOR[]; //DEFINE THE COLOR OF THE BALL
extern volatile char CAR_COLOR[];  //DEFINE THE COLOR OF THE CAR
extern volatile int MIDLINE_X;         //DEFINE THE LOCATION OF THE MIDDLE LINE
extern volatile int ALARM_X;           //DEFINE THE LOCATION OF THE ALARM LINE; WHEN THE BALL REACHES THIS LINE, THE CAR WILL START MOTION
extern volatile int CAR_ORI_X;         //DEFINE THE ORIGINAL POINT(X CORDINATE) OF THE CAR
extern volatile int CAR_ORI_Y;         //DEFINE THE ORIGINAL POINT(Y CORDINATE) OF THE CAR

extern volatile int L_NORMAL;
extern volatile int R_NORMAL;
//REFRESHING VALUES
extern volatile unsigned char receive;
extern volatile int counter;
extern volatile unsigned char buffer[50];
extern volatile int car_x_cord;
extern volatile int car_y_cord;
extern volatile int ball_x_cord;
extern volatile int ball_y_cord;

extern volatile int car_last_x_cord;
extern volatile int car_last_y_cord;
extern volatile int ball_last_x_cord;
extern volatile int ball_last_y_cord;
extern volatile int return_flag;

extern volatile double C_A;
extern volatile bool returned;
//@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@CARDINATE

double car_angle = 0;        //POSITIVE: 2/4  ;  NEGATIVE: 1/3
double ball_angle = 0;       //POSITIVE: 2/4  ;  NEGATIVE: 1/3
int car_dir = 0;             //0:FORWARD  ;  1:BACKWARD
int ball_dir = 0;            //0:FORWARD  ;  1:BACKWARD

double relative_angle = 0;   //RELATIVE ANGLE BETWEEN CAR DIRECTION AND BALL LOCATION; 
//THIS IS THE ANGLE THAT THE CAR SHOULD TURN;   POSITIVE: TURN LEFT ; NEGATIVE: TURN RIGHT

//@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@PID
double Kp = 600; //1500
double Ki = 0;
double Kd = 0;

double now_error = 0;
double now_output = 0;
double past_error = 0;
double Ts = 0;

//@@@@@@@@@@@@@@@@@@@@



double calculation_PID()
{
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
void relative_location(int x1, int y1, int x2, int y2)
{
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
int safe(int value)
{
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
double calculate_angle(int x1, int y1, int x2, int y2)
{
	double result = 1.0*atan2(abs_val(y1-y2),abs_val(x1-x2));
	if(CAR_NO == 0)
	{
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
	}
	else if(CAR_NO == 1)
	{
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
	}
}
double calculate_distance(int x1, int y1, int x2, int y2)
{
	return (sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

int abs_val(int input)
{
	if(input<0)
	{
		return (-input);
	}
	else
	{
	  return input;	
	}
}






















//END HERE