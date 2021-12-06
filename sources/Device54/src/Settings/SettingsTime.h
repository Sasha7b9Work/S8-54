﻿#pragma once
#include "SettingsTypes.h"
#include "defines.h"




/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsTime Settings Time
 *  @{
 */

#define SET_TSHIFT          (set.time_TShiftRel)
#define SET_TBASE           (set.time_TBase)
#define TIME_DIV_XPOS        (set.time_TimeDivXPos)

#define SET_PEAKDET         (set.time_PeakDet)
#define SET_PEAKDET_EN      (SET_PEAKDET == PeakDet_Enabled)
#define SET_PEAKDET_DIS     (SET_PEAKDET == PeakDet_Disabled)

#define IN_RANDOM_MODE      (SET_TBASE < TBase_50ns)
#define IN_P2P_MODE         (SET_TBASE >= MIN_TBASE_P2P)

#define TPOS                (set.time_TPos)
#define TPOS_IS_RIGHT       (TPOS == TPos_Right)
#define TPOS_IS_LEFT        (TPOS == TPos_Left)
#define TPOS_IS_CENTER      (TPOS == TPos_Center)

#define SAMPLE_TYPE         (set.time_SampleType)
#define SAMPLE_TYPE_IS_REAL (SAMPLE_TYPE == SampleType_Real)
#define SAMPLE_TYPE_OLD     (set.time_SampleTypeOld)

#define TPOS_IN_POINTS      sTime_TPosInPoints()
#define TSHIFT_IN_POINTS    sTime_TShiftInPoints()
#define TPOS_IN_BYTES       sTime_TPosInBytes()

#define LINKING_TSHIFT      (set.time_LinkingTShift)


/// Сохранить масштаб по времени
void sTime_SetTBase(TBase tBase);
/// Сохранить смещение по времени в относительных единицах
void sTime_SetTShift(int16 shift);
/// Узнать привязку отсительно уровня синхронизации в байтах
int sTime_TPosInBytes();
/// Минимальное смещение по времени, которое может быть записано в аппаратную часть
int16 sTime_TShiftMin();
/// Смещение по времени, соответствующее позиции TPos
int16 sTime_TShiftZero();

int sTime_TShiftInPoints();

int sTime_TPosInPoints();


/** @}  @}
 */
