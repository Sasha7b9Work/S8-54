#include "defines.h"
#pragma clang diagnostic ignored "-Wpadded"
#include <usbh_core.h>
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"


#ifdef __cplusplus
extern "C" {
#endif

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }
    
    //------------------------------------------------------------------------------------------------------------------------------------------------
    // This interrupt call soft NSS for spi (see Hardware::SPIforPanel.c::PanelInit() and HAL_GPIO_EXTI_Callback())
    void EXTI9_5_IRQHandler()
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    }
    
    //------------------------------------------------------------------------------------------------------------------------------------------------
    // See Hardware::SPIforPanel.c::HAL_SPI_RxCpltCallback()
    void SPI1_IRQHandler()
    {
        CPU::Panel::SPI_IRQHandler();
    }
    
    //------------------------------------------------------------------------------------------------------------------------------------------------
    void OTG_FS_IRQHandler()
    {
        HAL_HCD_IRQHandler(&CPU::FDrive::handleHCD);
    }

#ifdef __cplusplus
}
#endif
