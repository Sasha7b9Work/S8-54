#pragma once


#ifdef __cplusplus
extern "C" {
#endif
    

void SysTick_Handler(void);
void LTDC_IRQHandler(void);
void DMA2D_IRQHandler(void);
void OTG_FS_IRQHandler(void);


#ifdef __cplusplus
}
#endif
