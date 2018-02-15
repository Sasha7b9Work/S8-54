#include "defines.h"
#include "globals.h"
#include "Log.h"
#include "Sound.h"
#include "stub.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define POINTS_IN_PERIOD_SOUND 10
static uint8 points[POINTS_IN_PERIOD_SOUND] = {0};
static float frequency = 0.0f;
static float amplitude = 0.0f;
static TypeWave typeWave = TypeWave_Sine;
static bool soundWarnIsBeep = false;
static bool buttonIsPressed = false;    ///< \brief Когда запускается звук нажатой кнопки, устанавливается этот флаг, чтобы знать, проигрывать ли знак 
                                        ///< отпускания
static bool isBeep = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sound::Init()
{
    __DMA1_CLK_ENABLE();        // Для DAC1 (бикалка)
    __TIM7_CLK_ENABLE();        // Для DAC1 (бикалка)


    DAC_ChannelConfTypeDef config =
    {
        DAC_TRIGGER_T7_TRGO,
        DAC_OUTPUTBUFFER_ENABLE
    };

    HAL_DAC_DeInit(&handleDAC);

    HAL_DAC_Init(&handleDAC);

    HAL_DAC_ConfigChannel(&handleDAC, &config, DAC_CHANNEL_1);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Stop()
{
    HAL_DAC_Stop_DMA(&handleDAC, DAC_CHANNEL_1);
    isBeep = false;
    soundWarnIsBeep = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void TIM7_Config(uint16 prescaler, uint16 period)
{
    static TIM_HandleTypeDef htim =
    {
        TIM7, {}, HAL_TIM_ACTIVE_CHANNEL_1, {}, HAL_UNLOCKED, HAL_TIM_STATE_RESET
    };

    htim.Init.Prescaler = prescaler;
    htim.Init.Period = period;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&htim);

    TIM_MasterConfigTypeDef masterConfig =
    {
        TIM_TRGO_UPDATE,
        TIM_MASTERSLAVEMODE_DISABLE
    };

    HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);

    HAL_TIM_Base_Stop(&htim);
    HAL_TIM_Base_Start(&htim);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint16 CalculatePeriodForTIM()
{
#define MULTIPLIER_CALCPERFORTIM 30e6f

    return (uint16)(MULTIPLIER_CALCPERFORTIM / frequency / POINTS_IN_PERIOD_SOUND);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void CalculateSine()
{
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        float step = 2.0f * PI / (POINTS_IN_PERIOD_SOUND - 1);
        float value = (sinf(i * step) + 1.0f) / 2.0f;
        float v = value * amplitude * 255.0f;
        points[i] = (uint8)v;
    }

    for (int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        if (i < POINTS_IN_PERIOD_SOUND / 2)
        {
            points[i] = 0;
        }
        else
        {
            points[i] = (uint8)(255.0f * amplitude);
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void CalculateMeandr()
{
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND / 2; i++)
    {
        points[i] = (uint8)(255.0f * amplitude);
    }
    for(int i = POINTS_IN_PERIOD_SOUND / 2; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        points[i] = 0;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void CalculateTriangle()
{
    float k = 255.0 / POINTS_IN_PERIOD_SOUND;
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        points[i] = (uint8)(k * (float)i * amplitude);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SetWave()
{
    TIM7_Config(0, CalculatePeriodForTIM());

    if(typeWave == TypeWave_Sine)
    {
        CalculateSine();
    }
    else if(typeWave == TypeWave_Meandr)
    {
        CalculateMeandr();
    }
    else if(typeWave == TypeWave_Triangle)
    {
        CalculateTriangle();
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Sound_Beep(const TypeWave newTypeWave, const float newFreq, const float newAmpl, const int newDuration)
{
    if (soundWarnIsBeep)
    {
        return;
    }
    if (!SOUND_ENABLED)
    {
        return;
    }
    if (frequency != newFreq || amplitude != newAmpl || typeWave != newTypeWave)
    {
        frequency = newFreq;
        amplitude = newAmpl * SOUND_VOLUME / 100.0f;
        typeWave = newTypeWave;
        
        Stop();
        SetWave();
    }

    Stop();
    
    isBeep = true;
    
    HAL_DAC_Start_DMA(&handleDAC, DAC_CHANNEL_1, (uint32_t*)points, POINTS_IN_PERIOD_SOUND, DAC_ALIGN_8B_R);

    Timer::SetAndStartOnce(kStopSound, Stop, (uint)newDuration);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::ButtonPress()
{
    Sound_Beep(TypeWave_Sine, 2000.0f, 0.75f, 50);
    buttonIsPressed = true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::ButtonRelease()
{
    if (buttonIsPressed)
    {
        Sound_Beep(TypeWave_Sine, 1000.0f, 0.5f, 50);
        buttonIsPressed = false;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::GovernorChangedValue()
{
    Sound_Beep(TypeWave_Sine, 1000.0f, 0.5f, 50);
    buttonIsPressed = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::RegulatorShiftRotate()
{
    Sound_Beep(TypeWave_Sine, 1.0f, 0.2f, 3);
    buttonIsPressed = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::RegulatorSwitchRotate()
{
    Sound_Beep(TypeWave_Sine, 500.0f, 0.5f, 75);
    buttonIsPressed = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::WarnBeepBad()
{
    Sound_Beep(TypeWave_Meandr, 500.0f, 1.0f, 500);
    soundWarnIsBeep = true;
    buttonIsPressed = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::WarnBeepGood()
{
    Sound_Beep(TypeWave_Triangle, 1000.0f, 1.0f, 500);
    buttonIsPressed = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::WaitCompletion()
{
    while (isBeep)
    {

    };
}
