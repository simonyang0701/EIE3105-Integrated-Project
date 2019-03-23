#include "stm32f10x.h"                  // Device header
#include "PinMap.h"
#include "stdio.h"
#include "misc.h"

volatile uint32_t status = 0;
extern volatile uint16_t ADC_values[ARRAYSIZE];

int main(void) {
	
	char buffer[50] = {'\0'};
	
	USART2_init();
	ADC1_3channels_init();
	TIM3_PWM_CH1_init();
	DMA1_init();
	
	// start conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// start conversion (will be endless as we are in continuous mode)
	
	while(1) {
		while(!status);
		sprintf(buffer, "ch0=%d ch1=%d ch4=%d\r\n", ADC_values[0], ADC_values[1], ADC_values[2]);
		USARTSend(buffer, sizeof(buffer));
		TIM_SetCompare1(TIM3, ADC_values[0]);
		TIM_SetCompare2(TIM3, ADC_values[1]);
		TIM_SetCompare3(TIM3, ADC_values[2]);
		status = 0;
	}
}

void DMA1_Channel1_IRQHandler(void)
{
  //Test on DMA1 Channel1 Transfer Complete interrupt
  if(DMA_GetITStatus(DMA1_IT_TC1))
  {
	  status=1;
		
   //Clear DMA1 interrupt pending bits
    DMA_ClearITPendingBit(DMA1_IT_GL1);
  }
}
