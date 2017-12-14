#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup Measures
 *  @{
 */

// Виды измерений
typedef enum
{
    Meas_None,
    Meas_VoltageMax,
    Meas_VoltageMin,
    Meas_VoltagePic,
    Meas_VoltageMaxSteady,
    Meas_VoltageMinSteady,
    Meas_VoltageAmpl,
    Meas_VoltageAverage,
    Meas_VoltageRMS,
    Meas_VoltageVybrosPlus,
    Meas_VoltageVybrosMinus,
    Meas_Period,
    Meas_Freq,
    Meas_TimeNarastaniya,
    Meas_TimeSpada,
    Meas_DurationPlus,
    Meas_DurationMinus,
    Meas_SkvaznostPlus,
    Meas_SkvaznostMinus,
    Meas_DelayPlus,
    Meas_DelayMinus,
    Meas_PhazaPlus,
    Meas_PhazaMinus,
    Meas_NumMeasures
} Meas;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Measures
{
public:
    static char GetChar(Meas measure);

    static bool IsActive(int row, int col);

    static void GetActive(int *row, int *col);

    static void SetActive(int row, int col);

    static int GetDY();

    static int GetDX();

    static const char *Name(int row, int col);

    static Meas Type(int row, int col);

    static int NumRows();

    static int NumCols();

    static int GetTopTable();

    static int GetDeltaGridLeft();

    static int GetDeltaGridBottom();

    static void ShortPressOnSmallButonMarker();
    /// Нарисовать страницу выбора измерений
    static void DrawPageChoice();
};


/** @}  @}
 */
