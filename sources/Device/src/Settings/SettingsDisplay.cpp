#include "Settings.h"
#include "SettingsDisplay.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sDisplay_NumSignalsInS()
{
    return Tables_ENUMtoENumSignalsInSec(ENUM_SIGNALS_IN_SEC);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void sDisplay_SetNumSignalsInS(int numSignalsInS)
{
    ENUM_SIGNALS_IN_SEC = Tables_ENumSignalsInSecToENUM(numSignalsInS);
}
 

//----------------------------------------------------------------------------------------------------------------------------------------------------
int sDisplay_NumPointSmoothing()
{
    if(MODE_EMS && (ENUM_SMOOTHING < ENumSmooth_4points))
    {
        return 4;
    }

    int retValue = 0;
    if (SMOOTHING_ENABLED)
    {
        retValue = ENUM_SMOOTHING + 1;
    }

    if (IN_RANDOM_MODE)
    {
        int numRand = 0;
        if (NRST_NUM_SMOOTH_FOR_RAND > 1)
        {
            numRand = NRST_NUM_SMOOTH_FOR_RAND;
        }

        LIMIT_BELOW(retValue, numRand);
    }


    return retValue;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
int sDisplay_NumAverage()
{
    if (MODE_EMS && (ENUM_AVE < ENumAverages_8))
    {
        return 8;
    }

    if (!IN_RANDOM_MODE)
    {
        return NUM_AVE;
    }
    else if (NRST_NUM_AVE_FOR_RAND > NUM_AVE)
    {
        return NRST_NUM_AVE_FOR_RAND;
    }

    return NUM_AVE;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
BitSet64 sDisplay_PointsOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = (SET_PEAKDET_EN ? (SHIFT_IN_MEMORY / 2) : SHIFT_IN_MEMORY);
    retValue.sword1 = retValue.sword0 + 281;

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
BitSet64 sDisplay_BytesOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = SHIFT_IN_MEMORY;
    retValue.sword1 = retValue.sword0 + (SET_PEAKDET_EN ? 281 * 2 : 281);

    return retValue;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int sDisplay_ShiftInMemoryInPoints()
{
    if (SET_PEAKDET_DIS)
    {
        return SHIFT_IN_MEMORY;
    }
    return SHIFT_IN_MEMORY / 2;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int sDisplay_TimeMenuAutoHide()
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[MENU_AUTO_HIDE] * 1000;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool sDisplay_IsSeparate()
{
    return FUNC_MODE_DRAW_IS_SEPARATE || FFT_ENABLED;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void sDisplay_SetBrightness(int16 brightness)
{
    BRIGHTNESS_DISPLAY = brightness;
    Painter::SetBrightnessDisplay(brightness);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
ModeAveraging sDisplay_GetModeAveraging()
{
    if (IN_RANDOM_MODE)
    {
        return Averaging_Accurately;
    }
    return MODE_AVERAGING;
}
