#pragma once

#define GPIO_PIN_11 11

#define GPIO_MODE_INPUT 0

#define GPIO_PULLUP 0

#define GPIOG 0

struct HCD_HandleTypeDef
{
};


struct SPI_HandleTypeDef
{
};


struct DMA_HandleTypeDef
{
};


struct ADC_HandleTypeDef
{
};


struct DAC_HandleTypeDef
{
};

struct GPIO_InitTypeDef
{
    int field1;
    int field2;
    int field3;
};


void HAL_GPIO_Init(int, GPIO_InitTypeDef *);

unsigned int HAL_GetTick(void);
