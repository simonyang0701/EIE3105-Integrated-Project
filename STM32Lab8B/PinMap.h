#ifndef _PINMAP_H
#define _PINMAP_H
// Pin Usage
// Function      **  Pin Name ** Board Pin Out
// ADC1_IN0      **  PA0      ** A0
// ADC1_IN1      **  PA1      ** A1
// ADC1_IN4      **  PA4      ** A2
// TIM3 CH1 PWM  **  PA6      ** D12
// TIM3 CH2 PWM  **  PA7      ** D11
// TIM3 CH3 PWM  **  PB0      ** A3


// ADC1_IN0      **  PA0      ** A0
#define ADC1_0_RCC_GPIO  RCC_APB2Periph_GPIOA
#define ADC1_0_GPIO      GPIOA
#define ADC1_0_PIN       GPIO_Pin_0

// ADC1_IN1      **  PA1      ** A1
#define ADC1_1_RCC_GPIO  RCC_APB2Periph_GPIOA
#define ADC1_1_GPIO      GPIOA
#define ADC1_1_PIN       GPIO_Pin_1

// ADC1_IN4      **  PA4      ** A2
#define ADC1_4_RCC_GPIO  RCC_APB2Periph_GPIOA
#define ADC1_4_GPIO      GPIOA
#define ADC1_4_PIN       GPIO_Pin_4

#define ADC1_0_1_4_GPIO  GPIOA

// TIM3 CH1 PWM  **  PA6      ** D12
#define TIM3_CH1_PWM_RCC_GPIO  RCC_APB2Periph_GPIOA
#define TIM3_CH1_PWM_GPIO      GPIOA
#define TIM3_CH1_PWM_PIN       GPIO_Pin_6
// TIM3 CH2 PWM  **  PA7      ** D11
#define TIM3_CH2_PWM_RCC_GPIO  RCC_APB2Periph_GPIOA
#define TIM3_CH2_PWM_GPIO      GPIOA
#define TIM3_CH2_PWM_PIN       GPIO_Pin_7
// TIM3 CH3 PWM  **  PB0      ** A3
#define TIM3_CH3_PWM_RCC_GPIO  RCC_APB2Periph_GPIOB
#define TIM3_CH3_PWM_GPIO      GPIOB
#define TIM3_CH3_PWM_PIN       GPIO_Pin_0

//For debugging******************************
//LD2 PA5
#define LD2_RCC_GPIO  RCC_APB2Periph_GPIOA
#define LD2_GPIO      GPIOA
#define LD2_PIN       GPIO_Pin_5
//*******************************************

#define ARRAYSIZE 3
#define ADC1_DR    ((uint32_t)0x4001244C)

//Funcation prototypes
void ADC1_1channel_init(void);
void ADC1_3channels_init(void);
void DMA1_init(void);




void TIM3_PWM_CH1_init(void);
void USART2_init(void);
void USARTSend(char *pucBuffer, unsigned long ulCount);

#endif
