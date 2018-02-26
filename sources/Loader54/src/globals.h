#pragma once

#include <usbh_def.h>

#pragma once

extern SPI_HandleTypeDef   handleSPI;

#ifdef STM32F746xx
extern DMA2D_HandleTypeDef handleDMA2D;
extern LTDC_HandleTypeDef  handleLTDC;
#endif
