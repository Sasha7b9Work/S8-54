#pragma clang diagnostic ignored "-Wpadded"
#ifdef STM32F207xx
#include <stm32f2xx.h>
#else
#include <stm32f4xx.h>
#endif
#pragma clang diagnostic warning "-Wpadded"
#include "stm32.h"
#include "Hardware/CPU.h"


#ifdef __cplusplus
extern "C" {
#endif
  
    //----------------------------------------------------------------------------------------------------------------------------------------------------
    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }

#ifdef __cplusplus
}
#endif
