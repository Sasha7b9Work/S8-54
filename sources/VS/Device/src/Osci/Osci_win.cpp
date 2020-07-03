#include "defines.h"
#include "Hardware/Timer.h"
#include "Osci/Osci_win.h"
#include "Osci/ParametersOsci.h"
#include "Settings/Settings.h"


bool OsciHAL::ReadyPoint()
{
    static uint timeLastRead = 0;   // Время последнего чтения точки

    if(TIME_MS - timeLastRead >= TBase::TimePoint(S_TIME_BASE))
    {
        timeLastRead = TIME_MS;
        return true;
    }

    return false;
}
