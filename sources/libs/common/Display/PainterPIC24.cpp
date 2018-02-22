#include "Painter.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::SendToDisplay(uint8 *bytes, int numBytes)
{
    for (int i = 0; i < numBytes; i += 4)
    {
        while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
        {
        };

        Timer::PauseOnTicks(75);    /// \todo Здесь время ожидание увеличено по сравнению с С8-53 (там частота 120МГц, здесь - 180МГц)

        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
    }
}
