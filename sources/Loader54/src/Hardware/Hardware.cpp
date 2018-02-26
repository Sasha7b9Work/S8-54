#include "defines.h"
#include "it.h"
#include "FlashDrive/FlashDrive.h"
#include "Display/Display.h"
#include "Hardware.h"
#include "Hardware/FLASH.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Panel/Panel.h"
#include <stm32f437xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_dac.h>
#include "CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hardware::Init()
{
    CPU::Config();

    Timer::Init();
  
    Panel::Init();
}


