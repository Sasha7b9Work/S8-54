#pragma once


#ifdef __cplusplus
extern "C" {
#endif

    
void NMI_Handler();

void HardFault_Handler();

void MemManage_Handler();

void BusFault_Handler();

void UsageFault_Handler();

void SVC_Handler();

void DebugMon_Handler();

void PendSV_Handler();

void SysTick_Handler();
/// See Hardware::SPIforPanel.c::HAL_SPI_RxCpltCallback()
void SPI1_IRQHandler();
/// Прерывание для флешки
void OTG_FS_IRQHandler();

void OTG_HS_IRQHandler();
/// Используется для чтения АЦП рандомизатора
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

void TIM3_IRQHandler();

void ADC_IRQHandler();
/// This interrupt call soft NSS for spi (see Hardware::SPIforPanel.c::PanelInit() and HAL_GPIO_EXTI_Callback())
void EXTI9_5_IRQHandler();

void DMA1_Stream5_IRQHandler();

void DMA2_Stream0_IRQHandler();
/// Внешнее прерывание для чтения флага готовности точки в режиме поточечного вывода (Pin 116 - PD2)
void EXTI2_IRQHandler();


#ifdef __cplusplus
}
#endif
