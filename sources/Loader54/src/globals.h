#ifdef STM32F437xx
#include <stm32f4xx_hal.h>
#else
#include <stm32f7xx_hal.h>
#endif
#include <usbh_def.h>

#pragma once

extern HCD_HandleTypeDef   handleHCD;
extern USBH_HandleTypeDef  hUSB_Host;
extern SPI_HandleTypeDef   handleSPI;

#ifdef STM32F746xx
extern DMA2D_HandleTypeDef handleDMA2D;
extern LTDC_HandleTypeDef  handleLTDC;
#endif