#pragma once
#include "Generator.h"
#include "FPGA/FPGAMath.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stdlib.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SetParametersWave(Channel ch, TypeWave typeWave, float frequency, float startAngle, float amplWave, float amplNoise);
static void StartNewWave(Channel ch);
static uint8 GetSampleWave(Channel ch);


const SGenerator Generator = 
{
    SetParametersWave,
    StartNewWave,
    GetSampleWave,
};


static float NewNoiseValue(Channel ch);
static uint8 GetSampleSinusWave(Channel ch, int numSample);
static uint8 GetSampleMeanderWave(Channel ch, int numSample);


static TypeWave type[2] = {Wave_Sinus, Wave_Meander};
static float freq[2] = {1000.0f, 500.0f};
static float angle[2] = {0.05f, 0.1f};
static float ampl[2] = {1.0f, 0.5f};
static float amplNoise[2] = {0.1f, 0.1f};
static int nSample[2] = {0, 0};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetParametersWave(Channel ch, TypeWave typeWave, float frequency, float startAngle, float amplWave, float amplNoise_)
{
    type[ch] = typeWave;
    freq[ch] = frequency;
    angle[ch] = startAngle;
    ampl[ch] = amplWave;
    amplNoise[ch] = amplNoise_;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void StartNewWave(Channel)
{
    nSample[0] = nSample[1] = 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 GetSampleWave(Channel ch)
{
    return (type[ch] == Wave_Sinus) ? GetSampleSinusWave(ch, (nSample[ch])++) : GetSampleMeanderWave(ch, (nSample[ch])++);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 GetSampleSinusWave(Channel ch, int numSample)
{
    float dT = numSample * TSHIFT_2_ABS(1, SET_TBASE);
    float voltage = ampl[ch] * sinf(2.0f * PI * freq[ch] * dT + angle[ch]) + NewNoiseValue(ch);
    return mathFPGA.Voltage2Point(voltage, SET_RANGE(ch), (int16)SET_RSHIFT(ch));
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 GetSampleMeanderWave(Channel, int)
{
    return 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float NewNoiseValue(Channel ch)
{
    static float prevNoise[2] = {0.0f, 0.0f};            // Здесь хранятся значения шума из предыдущих точек, необходимые для расчёта шума в текущей точке.

    float noise = prevNoise[ch];

    float halfAmplNoiseAbs = ampl[ch] * amplNoise[ch] / 2.0f;

    float deltaRand = halfAmplNoiseAbs;

    noise += math.RandFloat(-deltaRand, deltaRand);

    while (noise < -halfAmplNoiseAbs)
    {
        noise += math.RandFloat(0, deltaRand * 2);
    }

    while (noise > halfAmplNoiseAbs)
    {
        noise -= math.RandFloat(0, deltaRand * 2);
    }

    prevNoise[ch] = noise;

    return noise;
}
