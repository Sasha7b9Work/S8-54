// 2024/01/05 21:40:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "stm32f4xx_hal.h"


void HAL_DMA_Start_IT(DMA_HandleTypeDef *, unsigned int, unsigned int, unsigned int)
{

}


void HAL_DMA_DeInit(DMA_HandleTypeDef *)
{

}


int HAL_DMA_Init(DMA_HandleTypeDef *)
{
    return HAL_OK;
}


int HAL_RTC_Init(RTC_HandleTypeDef *)
{
    return HAL_OK;
}


int HAL_RTCEx_BKUPRead(RTC_HandleTypeDef *, int)
{
    return HAL_OK;
}


int HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *, int, int)
{
    return HAL_OK;
}


void HAL_RTC_GetTime(RTC_HandleTypeDef *, RTC_TimeTypeDef *, int)
{

}


void HAL_RTC_GetDate(RTC_HandleTypeDef *, RTC_DateTypeDef *, int)
{

}


int HAL_RTC_SetDate(RTC_HandleTypeDef *, RTC_DateTypeDef *, int)
{
    return HAL_OK;
}


int HAL_RTC_SetTime(RTC_HandleTypeDef *, RTC_TimeTypeDef *, int)
{
    return HAL_OK;
}


void HAL_RTCEx_SetCoarseCalib(RTC_HandleTypeDef *, unsigned int, unsigned int)
{

}


void HAL_FLASH_Unlock()
{

}


int HAL_FLASH_Program(int, unsigned int, unsigned long long)
{
    return HAL_OK;
}


void HAL_FLASH_Lock()
{

}


void HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *, unsigned int *)
{

}


void HAL_Init()
{

}


void HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *, int)
{

}


void HAL_RCC_OscConfig(RCC_OscInitTypeDef *)
{

}


int HAL_CRC_Init(CRC_HandleTypeDef *)
{
    return HAL_OK;
}


unsigned int HAL_CRC_Calculate(CRC_HandleTypeDef *, unsigned int *, int)
{
    return 0;
}


uint HAL_GetTick()
{
    return 0;
}


void HAL_TIM_Base_Start(TIM_HandleTypeDef *)
{

}


void HAL_TIM_Base_Stop(TIM_HandleTypeDef *)
{

}


void HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *, TIM_MasterConfigTypeDef *)
{

}


void HAL_TIM_Base_Init(TIM_HandleTypeDef *)
{

}


void HAL_DAC_Stop_DMA(DAC_HandleTypeDef *, int)
{

}


void HAL_DAC_ConfigChannel(DAC_HandleTypeDef *, DAC_ChannelConfTypeDef *, int)
{

}


void HAL_DAC_Init(DAC_HandleTypeDef *)
{

}


void HAL_DAC_DeInit(DAC_HandleTypeDef *)
{

}


void HAL_DAC_Start_DMA(DAC_HandleTypeDef *, int, unsigned int *, int, int)
{

}


void HAL_SPI_DeInit(SPI_HandleTypeDef *)
{

}


void HAL_SPI_Receive_IT(SPI_HandleTypeDef *, unsigned char *, int)
{

}


int HAL_SPI_Init(SPI_HandleTypeDef *)
{
    return HAL_OK;
}
