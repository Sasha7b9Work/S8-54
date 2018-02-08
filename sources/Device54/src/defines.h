#pragma once

#ifdef STM32F437xx
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f4xx_hal.h>
#pragma clang diagnostic warning "-Wc++98-compat-pedantic"
#pragma clang diagnostic warning "-Wpadded"
#endif

#ifdef STM32F746xx
#include <stm32f7xx_hal.h>
#endif

#include "commonDefines.h"

#define NUM_VER "1.3"

#define INTERRUPT_P2P   // Если определено, то для чтения точек используется прерывание
 
#define OLD_RECORDER

#define DEBUG
