#include "defines.h"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#include <stm32f4xx.h>
#include <usbh_core.h>
#pragma clang diagnostic warning "-Wpadded"
#pragma clang diagnostic warning "-Wc++98-compat-pedantic"
#include "CPU.h"
#include "FlashDrive/FlashDrive.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Init()
{
    STM437::Init();

    EnablePeriphery();

    InitHardware();

    // Таймер для мс
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::DeInit()
{
    Panel::DeInit();
    
    HAL_DeInit();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::EnablePeriphery()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();        // Для DAC1 (бикалка)
    __HAL_RCC_TIM7_CLK_ENABLE();        // Для DAC1 (бикалка)
    __HAL_RCC_DAC_CLK_ENABLE();         // Бикалка
    __PWR_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::InitHardware()
{
    FSMC_::Init();
    
    Panel::Init();
    
    //FDrive::Init();
}
