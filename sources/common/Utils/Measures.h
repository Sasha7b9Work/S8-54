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
    VoltageMax,
    VoltageMin,
    VoltagePic,
    VoltageMaxSteady,
    VoltageMinSteady,
    VoltageAmpl,
    VoltageAverage,
    VoltageRMS,
    VoltageVybrosPlus,
    VoltageVybrosMinus,
    Period,
    Freq,
    TimeNarastaniya,
    TimeSpada,
    DurationPlus,
    DurationMinus,
    SkvaznostPlus,
    SkvaznostMinus,
    DelayPlus,
    DelayMinus,
    PhazaPlus,
    PhazaMinus,
    NumMeasures
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
