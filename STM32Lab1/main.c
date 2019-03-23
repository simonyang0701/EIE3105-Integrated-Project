#include "stm32f10x.h"

#define GPIO_LED GPIO_Pin_5
#define GPIO_BUT GPIO_Pin_13

void lightToMe() {
    // initializing a structure
    GPIO_InitTypeDef GPIO_InitStructure;

    // enabling all GPIO clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // setting the GPIO_Speed by default (2MHz)
    GPIO_StructInit(&GPIO_InitStructure);

    // setting the LCD pin
    GPIO_InitStructure.GPIO_Pin = GPIO_LED;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // setting the USER BUTTON pin
    GPIO_InitStructure.GPIO_Pin = GPIO_BUT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // infinite loop
    while (1) {
        if (GPIO_ReadInputDataBit(GPIOC, GPIO_BUT)) {
            GPIO_WriteBit(GPIOA, GPIO_LED, Bit_RESET);
        } else {
            GPIO_WriteBit(GPIOA, GPIO_LED, Bit_SET);
        }
    }
}

int main(void) {
    lightToMe();
    return 0;
}