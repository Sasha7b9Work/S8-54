 #pragma once
#include "defines.h"


#define ADDR_BANK           0x60000000

#define ADDR_CDISPLAY               ((uint8 *)(ADDR_BANK + 0x080000))    // Здесь 8-битный низкоскоростной мультиплексный режим
#define ADDR_FPGA                   ((uint8 *)(ADDR_BANK + 0x300000))    // Здесь 16-битный высокоскоростной немульиплексный режим
#define ADDR_RAM                    ((uint8 *)(ADDR_BANK + 0x200000))    // Здесь 16-битный высокоскоростной немульиплексный режим


typedef enum
{
    ModeFSMC_None,
    ModeFSMC_Display,
    ModeFSMC_FPGA,
    ModeFSMC_RAM
} ModeFSMC;


class FSMC
{
public:
    static void Init();

    static void SetMode(ModeFSMC mode);
};
