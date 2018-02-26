#include "defines.h"
#include "CPU.h"
#include "it.h"
#include "FlashDrive/FlashDrive.h"
#include "Display/Display.h"
#include "Hardware.h"
#include "Hardware/FLASH.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Panel/Panel.h"
#include <stm32f437xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_dac.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hardware::Init()
{
    CPU::Config();

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

    // Timer  /////////////////////////////////////////////////////////////////
    //RCC_PCLK1Config(RCC_HCLK_Div1);

    // Таймер для мс
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, PRIORITY_TIMER_TIM6);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    Timer::Init();
  
    Panel::Init();
}

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OTG_FS_IRQHandler()
{
    HAL_HCD_IRQHandler(&handleHCD);
}

#ifdef __cplusplus
}
#endif
