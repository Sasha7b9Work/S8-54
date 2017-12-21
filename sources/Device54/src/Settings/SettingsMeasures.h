#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsMeasures Settings Measures
 *  @{
 */

#define MODE_VIEW_SIGNALS               (set.meas_ModeViewSignals) 
#define MODE_VIEW_SIGNALS_IS_COMPRESS   (MODE_VIEW_SIGNALS == ModeViewSignals_Compress)  // —жимать ли сетку при выводе измерений
#define MEASURE(n)                      (set.meas_Measures[n])
#define SHOW_MEASURES                   (set.meas_Show)
#define MEAS_MARKED                     (set.meas_Marked)

#define MEAS_ZONE                       (set.meas_Zone)
#define MEAS_ZONE_HAND                  (MEAS_ZONE == MeasuresZone_Hand)

#define NUM_MEASURES                    (set.meas_Number)
#define NUM_MEASURES_IS_1_5             (NUM_MEASURES == MN_1_5)
#define NUM_MEASURES_IS_2_5             (NUM_MEASURES == MN_2_5)
#define NUM_MEASURES_IS_3_5             (NUM_MEASURES == MN_3_5)
#define NUM_MEASURES_IS_6_1             (NUM_MEASURES == MN_6_1)
#define NUM_MEASURES_IS_6_2             (NUM_MEASURES == MN_6_2)

#define SOURCE_MEASURE                  (set.meas_Source)
#define SOURCE_MEASURE_IS_A             (SOURCE_MEASURE == A)
#define SOURCE_MEASURE_IS_B             (SOURCE_MEASURE == B)
#define SOURCE_MEASURE_IS_A_B           (SOURCE_MEASURE == A_B)

#define POS_MEAS_CUR_U(n)               (set.meas_PosCurU[n])
#define POS_MEAS_CUR_U_0                (POS_MEAS_CUR_U(0))
#define POS_MEAS_CUR_U_1                (POS_MEAS_CUR_U(1))

#define POS_MEAS_CUR_T(n)               (set.meas_PosCurT[n])
#define POS_MEAS_CUR_T_0                (POS_MEAS_CUR_T(0))
#define POS_MEAS_CUR_T_1                (POS_MEAS_CUR_T(1))

/** @}  @}
 */
