#pragma once

#define ADC_SAMPLETIME_28CYCLES 0
#define ADC_CHANNEL_4 0
#define HAL_OK 1
#define ADC_EXTERNALTRIGCONV_Ext_IT11 1
#define ADC_EXTERNALTRIGCONVEDGE_RISING 1
#define GPIO_PIN_2   2
#define GPIO_PIN_3   3
#define GPIO_PIN_6   6
#define GPIO_PIN_10 10
#define GPIO_PIN_11 11
#define GPIO_PIN_12 11
#define GPIO_PIN_15 15
#define GPIO_MODE_INPUT     0
#define GPIO_MODE_IT_RISING 0
#define GPIO_MODE_ANALOG    0
#define GPIO_PULLUP 0
#define GPIO_NOPULL 0
#define GPIOC 0
#define GPIOD 0
#define GPIOF 0
#define GPIOG 0
#define EXTI2_IRQn 0
#define ADC_IRQn   0
#define GPIO_PIN_RESET 0
#define GPIO_PIN_SET   1
#define __HAL_RCC_GPIOD_CLK_ENABLE()
#define __ADC3_CLK_ENABLE()
#define ADC3 3
#define ADC_CLOCKPRESCALER_PCLK_DIV2 0
#define ADC_RESOLUTION12b 0
#define ADC_DATAALIGN_RIGHT 0
#define DISABLE 0
#define ENABLE 0

struct ADC_ChannelConfTypeDef
{
    int Channel;
    int Rank;
    int SamplingTime;
    int Offset;
};

struct HCD_HandleTypeDef
{
};


struct SPI_HandleTypeDef
{
};


struct DMA_HandleTypeDef
{
};


struct ADC_InitTypeDef
{
    int ClockPrescaler;
    int Resolution;
    int DataAlign;
    int ScanConvMode;
    int EOCSelection;
    int ContinuousConvMode;
    int DMAContinuousRequests;
    int NbrOfConversion;
    int DiscontinuousConvMode;
    int NbrOfDiscConversion;
    int ExternalTrigConvEdge;
    int ExternalTrigConv;
};

struct ADC_HandleTypeDef
{
    int Instance;
    ADC_InitTypeDef Init;
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

struct GPIO_TypeDef
{
};


int HAL_ADC_Start_IT(ADC_HandleTypeDef *);
int HAL_ADC_ConfigChannel(ADC_HandleTypeDef *, ADC_ChannelConfTypeDef *);
void HAL_NVIC_DisableIRQ(int);
void HAL_NVIC_EnableIRQ(int);
void HAL_NVIC_SetPriority(int, int, int);
void HAL_GPIO_Init(int, GPIO_InitTypeDef *);
void HAL_GPIO_WritePin(GPIO_TypeDef *, int, int);
unsigned int HAL_GetTick(void);
int HAL_ADC_Init(ADC_HandleTypeDef *);
