/************************************************************
motion: motion related functions
************************************************************/
#include "Lunar_RBC.h"


void Start_Move(int Ld, int Rd)
{
	TIM_SetCompare1(TIM3,Ld);
	TIM_SetCompare2(TIM3,Rd);
}

void Stop_Move()
{
	TIM_SetCompare1(TIM3,1);
	TIM_SetCompare2(TIM3,1);
}

//void Turn(bool dir, int speed);

//void Change_Dir(bool wheel, bool dir);











//END HERE
