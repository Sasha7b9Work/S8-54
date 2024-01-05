#pragma once

#define FLASH_OTP_END 0
#define FLASH_OTP_BASE 0
#define TYPEPROGRAM_BYTE 0
#define FLASH_SECTOR_23 0
#define FLASH_SECTOR_22 0
#define FLASH_SECTOR_21 0
#define FLASH_SECTOR_20 0
#define FLASH_SECTOR_19 0
#define FLASH_SECTOR_18 0
#define FLASH_SECTOR_17 0
#define FLASH_SECTOR_16 0
#define FLASH_SECTOR_11 0
#define VOLTAGE_RANGE_3 0
#define TYPEERASE_SECTORS 0
#define TYPEPROGRAM_WORD 0
#define __HAL_FLASH_CLEAR_FLAG(x)
#define FLASH_FLAG_PGSERR 0
#define FLASH_FLAG_PGPERR 0
#define FLASH_FLAG_PGAERR 0
#define FLASH_FLAG_WRPERR 0
#define FLASH_FLAG_OPERR 0
#define FLASH_FLAG_EOP 0
#define RTC_CALIBSIGN_POSITIVE 0
#define RTC_CALIBSIGN_NEGATIVE 0
#define RTC_STOREOPERATION_SET 0
#define RTC_DAYLIGHTSAVING_NONE 0
#define RTC_WEEKDAY_MONDAY 0
#define FORMAT_BIN 0
#define RTC_BKP_DR0 0
#define DMA2_Stream0_IRQn 0
#define __HAL_RCC_DMA2_CLK_ENABLE()
#define DAC 0
#define SPI_CRCCALCULATION_DISABLED 0
#define SPI_TIMODE_DISABLED 0
#define SPI_FIRSTBIT_MSB 0
#define SPI_BAUDRATEPRESCALER_2 0
#define SPI_NSS_SOFT 0
#define SPI_PHASE_1EDGE 0
#define SPI_POLARITY_HIGH 0
#define SPI_DATASIZE_8BIT 0
#define SPI_DIRECTION_2LINES 0
#define SPI_MODE_SLAVE 0
#define SPI1 0
#define DMA_PBURST_SINGLE 0
#define DMA_MBURST_SINGLE 0
#define DMA_FIFO_THRESHOLD_FULL 0
#define DMA_FIFOMODE_DISABLE 0
#define DMA_PRIORITY_HIGH 0
#define DMA_NORMAL 0
#define DMA_MDATAALIGN_HALFWORD 9
#define DMA_PDATAALIGN_HALFWORD 0
#define DMA_MINC_ENABLE 0
#define DMA_PINC_ENABLE 0
#define DMA_MEMORY_TO_MEMORY 0
#define DMA_CHANNEL_0 0
#define DMA2_Stream0 0
#define EXTI9_5_IRQn 0
#define SPI1_IRQn 0
#define GPIOB 0
#define GPIOA 0
#define GPIO_AF5_SPI1 0
#define GPIO_SPEED_FAST 0
#define GPIO_PULLDOWN 0
#define GPIO_MODE_AF_PP 0
#define GPIO_PIN_5 5
#define __SPI1_CLK_ENABLE()
#define __HAL_RCC_GPIOB_CLK_ENABLE()
#define __HAL_RCC_GPIOA_CLK_ENABLE()
#define GPIO_AF0_MCO 0
#define GPIO_SPEED_HIGH 0
#define GPIO_MODE_OUTPUT_PP 0
#define SysTick_IRQn 0
#define __SYSCFG_CLK_ENABLE()
#define __PWR_CLK_ENABLE()
#define __DAC_CLK_ENABLE()
#define __TIM7_CLK_ENABLE()
#define __DMA1_CLK_ENABLE()
#define __GPIOG_CLK_ENABLE()
#define __GPIOF_CLK_ENABLE()
#define __GPIOE_CLK_ENABLE()
#define __GPIOD_CLK_ENABLE()
#define __GPIOC_CLK_ENABLE()
#define __GPIOB_CLK_ENABLE()
#define __GPIOA_CLK_ENABLE()
#define RCC_OSCILLATORTYPE_HSE 0
#define __HAL_PWR_VOLTAGESCALING_CONFIG(x)
#define PWR_REGULATOR_VOLTAGE_SCALE1
#define __HAL_RCC_PWR_CLK_ENABLE()
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
#define RCC_HSE_ON 0
#define RCC_PLL_ON 0
#define RCC_PLLSOURCE_HSE 0
#define RCC_CLOCKTYPE_SYSCLK 0
#define RCC_CLOCKTYPE_HCLK 0
#define RCC_CLOCKTYPE_PCLK1 0
#define RCC_CLOCKTYPE_PCLK2 0
#define RCC_SYSCLKSOURCE_PLLCLK 0
#define RCC_SYSCLK_DIV1 0
#define RCC_HCLK_DIV4 0
#define RCC_HCLK_DIV2 0
#define FLASH_LATENCY_5 0
#define RTC 0
#define RTC_HOURFORMAT_24 0
#define RTC_OUTPUT_DISABLE 0
#define RTC_OUTPUT_POLARITY_HIGH 0
#define RTC_OUTPUT_TYPE_OPENDRAIN 0


struct RTC_InitTypeDef
{
    int HourFormat;
    int AsynchPrediv;
    int SynchPrediv;
    int OutPut;
    int OutPutPolarity;
    int OutPutType;
};

struct RTC_HandleTypeDef
{
    int             Instance;
    RTC_InitTypeDef Init;
};

struct RCC_PLLInitTypeDef
{
    int PLLState;
    int PLLSource;
    int PLLM;
    int PLLN;
    int PLLP;
    int PLLQ;
};

struct RCC_ClkInitTypeDef
{
    int ClockType;
    int SYSCLKSource;
    int AHBCLKDivider;
    int APB1CLKDivider;
    int APB2CLKDivider;
};

struct RCC_OscInitTypeDef
{
    RCC_PLLInitTypeDef PLL;
    int OscillatorType;
    int HSEState;
};

struct CRC_HandleTypeDef
{
    int Instance;
};

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

struct SPI_InitTypeDef
{
    int Mode;
    int Direction;
    int DataSize;
    int CLKPolarity;
    int CLKPhase;
    int NSS;
    int BaudRatePrescaler;
    int FirstBit;
    int TIMode;
    int CRCCalculation;
    int CRCPolynomial;
};

struct SPI_HandleTypeDef
{
    int             Instance;
    SPI_InitTypeDef Init;
};

struct DMA_InitTypeDef
{
    int Channel;
    int Direction;
    int PeriphInc;
    int MemInc;
    int PeriphDataAlignment;
    int MemDataAlignment;
    int Mode;
    int Priority;
    int FIFOMode;
    int FIFOThreshold;
    int MemBurst;
    int PeriphBurst;
};

struct DMA_HandleTypeDef
{
    int             Instance;
    DMA_InitTypeDef Init;
    void (*XferCpltCallback)(DMA_HandleTypeDef *);
    void (*XferErrorCallback)(DMA_HandleTypeDef *);
};

struct USBD_HandleTypeDef
{
};

struct PCD_HandleTypeDef
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
    int Instance;
};

struct GPIO_InitTypeDef
{
    int Pin;
    int Mode;
    int field3;
    int Speed;
    int Alternate;
};

struct GPIO_TypeDef
{
};

struct RTC_TimeTypeDef
{
    unsigned int Hours;
    unsigned int Minutes;
    unsigned int Seconds;
    unsigned int TimeFormat;
    unsigned int DayLightSaving;
    unsigned int StoreOperation;
};

struct RTC_DateTypeDef
{
    unsigned int Year;
    unsigned int Month;
    unsigned int Date;
    unsigned int WeekDay;
};

struct FLASH_EraseInitTypeDef
{
    int TypeErase;
    unsigned int Sector;
    int NbSectors;
    int VoltageRange;
};


void HAL_Init();
void HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *, int);
void HAL_RCC_OscConfig(RCC_OscInitTypeDef *);
int HAL_ADC_Start_IT(ADC_HandleTypeDef *);
int HAL_ADC_ConfigChannel(ADC_HandleTypeDef *, ADC_ChannelConfTypeDef *);
void HAL_NVIC_DisableIRQ(int);
void HAL_NVIC_EnableIRQ(int);
void HAL_NVIC_SetPriority(int, int, int);
void HAL_GPIO_Init(int, GPIO_InitTypeDef *);
void HAL_GPIO_WritePin(GPIO_TypeDef *, int, int);
unsigned int HAL_GetTick(void);
int HAL_ADC_Init(ADC_HandleTypeDef *);
int HAL_CRC_Init(CRC_HandleTypeDef *);
unsigned int HAL_CRC_Calculate(CRC_HandleTypeDef *, unsigned int *, int);
int HAL_SPI_Init(SPI_HandleTypeDef *);
void HAL_SPI_Receive_IT(SPI_HandleTypeDef *, unsigned char *, int);
void HAL_SPI_DeInit(SPI_HandleTypeDef *);
void HAL_DMA_Start_IT(DMA_HandleTypeDef *, unsigned int, unsigned int, unsigned int);
void HAL_DMA_DeInit(DMA_HandleTypeDef *);
int HAL_DMA_Init(DMA_HandleTypeDef *);
int HAL_RTC_Init(RTC_HandleTypeDef *);
int HAL_RTCEx_BKUPRead(RTC_HandleTypeDef *, int);
int HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *, int, int);
void HAL_RTC_GetTime(RTC_HandleTypeDef *, RTC_TimeTypeDef *, int);
void HAL_RTC_GetDate(RTC_HandleTypeDef *, RTC_DateTypeDef *, int);
int HAL_RTC_SetDate(RTC_HandleTypeDef *, RTC_DateTypeDef *, int);
int HAL_RTC_SetTime(RTC_HandleTypeDef *, RTC_TimeTypeDef *, int);
void HAL_RTCEx_SetCoarseCalib(RTC_HandleTypeDef *, unsigned int, unsigned int);
void HAL_FLASH_Unlock();
int HAL_FLASH_Program(int, unsigned int, unsigned long long);
void HAL_FLASH_Lock();
void HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *, unsigned int *);
int HAL_GPIO_ReadPin(GPIO_InitTypeDef *, int);
