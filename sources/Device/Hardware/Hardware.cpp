#include "defines.h"
#include "Log.h"
#include "Display/Display.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware.h"
#include "Hardware/FLASH.h"
#include "Hardware/Sound.h"
#include "Hardware/RTC.h"
#include "Hardware/FSMC.h"
#include "Hardware/RAM.h"
#include "Hardware/Timer.h"
#include "FPGA/FPGA.h"
#include "Panel/Panel.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static CRC_HandleTypeDef crcHandle;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SystemClock_Config()
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    HAL_RCC_OscConfig (&RCC_OscInitStruct);
    
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK |RCC_CLOCKTYPE_HCLK |RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::Init()
{
    HAL_Init();

    SystemClock_Config();

   __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();
    __DMA1_CLK_ENABLE();        // Для DAC1 (бикалка)
    
    __TIM7_CLK_ENABLE();        // Для DAC1 (бикалка)
    __DAC_CLK_ENABLE();         // Для бикалки
    __PWR_CLK_ENABLE();

    __SYSCFG_CLK_ENABLE();

    HAL_NVIC_SetPriority(SysTick_IRQn, PRIORITY_SYS_TICK);

    Timer::Init();

    Sound::Init();
    
    Panel::Init();

    FSMC::Init();
    
    FSMC::SetMode(ModeFSMC_FPGA);

    RAM::Init();

    FDrive::Init();
    
// Analog and DAC programmable SPI ////////////////////////////////////////

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_10 | GPIO_PIN_12,          // SPI SCLK, DATA
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF0_MCO
    };
    HAL_GPIO_Init(GPIOC, &isGPIO);

    isGPIO.Pin = GPIO_PIN_3 | GPIO_PIN_6;   // SPI select1, select2
    HAL_GPIO_Init(GPIOD, &isGPIO);

    isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_15; // SPI select3, select4
    HAL_GPIO_Init(GPIOG, &isGPIO);

    RTClock::Init();

    crcHandle.Instance = CRC;
    if (HAL_CRC_Init(&crcHandle) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Hardware::CalculateCRC32()
{
    return HAL_CRC_Calculate(&crcHandle, (uint *)0x08020000, 128 * 1024 * 3 / 4);
}
