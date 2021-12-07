#include "FPGA/FPGA.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"


#ifdef __cplusplus
extern "C" {
#endif

    
    void HardFault_Handler()
    {
        while (1)
        {
        }
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }

    
    void ADC_IRQHandler()
    {
        HAL_ADC_IRQHandler(&handleADC);
    }
    
    
    
    /// This interrupt call soft NSS for spi (see Hardware::SPIforPanel.c::PanelInit() and HAL_GPIO_EXTI_Callback())
    void EXTI9_5_IRQHandler()
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    }
    
    
    
    /// See Hardware::SPIforPanel.c::HAL_SPI_RxCpltCallback()
    void SPI1_IRQHandler()
    {
        HAL_SPI_IRQHandler(&handleSPI);
    }
    
    
    
    /// ���������� ��� ������
    void OTG_FS_IRQHandler()
    {
        HAL_HCD_IRQHandler(&handleHCD);
    }
    
    
    
    void DMA1_Stream5_IRQHandler()
    {
        HAL_DMA_IRQHandler(handleDAC.DMA_Handle1);
    }
        
    
    
    void DMA2_Stream0_IRQHandler()
    {
        HAL_DMA_IRQHandler(&handleDMA_RAM);
    }
    
    
    
    /// ������� ���������� ��� ������ ����� ���������� ����� � ������ ����������� ������ (Pin 116 - PD2)
    void EXTI2_IRQHandler()
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
    }
    
    
    
    /// ������������ ��� ������ ��� �������������
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
    {
        FPGA::adcValueFPGA = (uint16)HAL_ADC_GetValue(hadc);
    }
    
    
    void OTG_HS_IRQHandler()
    {
        HAL_PCD_IRQHandler(&VCP::handlePCD);
    }

#ifdef __cplusplus
}
#endif
