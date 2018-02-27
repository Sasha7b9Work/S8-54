#pragma once

#ifndef __ALIGN_BEGIN
#define __ALIGN_BEGIN
#endif

#ifndef __ALIGN_END
#define __ALIGN_END __attribute__ ((aligned (4)))
#endif

#ifdef STM32F437xx
#include <stm32f4xx_hal.h>
#endif

#ifdef STM32F746xx
#include <stm32f7xx_hal.h>
#endif

#include <stdlib.h>
#include <string.h>

#define USBD_LPM_ENABLED 0
#define USE_RTOS         0U


#define USBD_MAX_NUM_INTERFACES               1
#define USBD_MAX_NUM_CONFIGURATION            1
#define USBD_MAX_STR_DESC_SIZ                 0x100
#define USBD_SUPPORT_USER_STRING              0
#define USBD_SELF_POWERED                     1
#define USBD_DEBUG_LEVEL                      0
 
#define USBD_malloc               malloc
#define USBD_free                 free
#define USBD_memset               memset
#define USBD_memcpy               memcpy


#if (USBD_DEBUG_LEVEL > 0)
#define USBD_UsrLog(...)  Log_Write(__VA_ARGS__)
#define USBD_ErrLog(...)  Log_Write(__VA_ARGS__)
#define USBD_DbgLog(...)  Log_Write(__VA_ARGS__)
#else
#define USBD_UsrLog(...) {}
#define USBD_ErrLog(...) {}
#define USBD_DbgLog(...) {}
#endif
