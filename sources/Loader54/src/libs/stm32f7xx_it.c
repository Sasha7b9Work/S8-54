#include "globals.h"
#include <stm32f7xx_hal.h>
#include <stm32f7xx.h>
#include <stm32f7xx_it.h>


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OTG_FS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&handleHCD);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LTDC_IRQHandler(void)
{
    HAL_LTDC_IRQHandler(&handleLTDC);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DMA2D_IRQHandler(void)
{
    HAL_DMA2D_IRQHandler(&handleDMA2D);
}

#ifdef __cplusplus
}
#endif
