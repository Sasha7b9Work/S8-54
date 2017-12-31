#pragma once

#ifdef STM32F437xx
#include <stm32f4xx_hal.h>
#endif

#ifdef STM32F746xx
#include <stm32f7xx_hal.h>
#endif

#include "commonDefines.h"

#define NUM_VER "1.3"

#define INTERRUPT_P2P   // Если определено, то для чтения точек используется прерывание
 
#define OLD_RECORDER

#define DEBUG
