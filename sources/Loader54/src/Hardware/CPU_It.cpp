#include "defines.h"
#pragma clang diagnostic ignored "-Wpadded"
#include <usbh_core.h>
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "CPU.h"


#ifdef __cplusplus
extern "C" {
#endif

void EXTI9_5_IRQHandler();
void SPI1_IRQHandler();

//----------------------------------------------------------------------------------------------------------------------------------------------------
// This interrupt call soft NSS for spi (see Hardware::SPIforPanel.c::PanelInit() and HAL_GPIO_EXTI_Callback())
void EXTI9_5_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
// See Hardware::SPIforPanel.c::HAL_SPI_RxCpltCallback()
void SPI1_IRQHandler()
{
    CPU::Panel::SPI_IRQHandler();
}

#ifdef __cplusplus
}
#endif
