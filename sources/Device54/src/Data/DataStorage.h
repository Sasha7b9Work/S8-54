#pragma once
#include "Settings/SettingsTypes.h"
#include "Hardware/RTC.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup FPGA
 *  @{
 *  @defgroup DataStorage Data Storage
 *  @{
 */

class Storage
{
public:
    /// Удаление всех сохранённых измерений.
    static void Clear();
    /// Добавить считанные данные. При этом настройками считаются текущие настройки прибора.
    static void AddData(uint8 *dataA, uint8 *dataB, DataSettings dss);
    /// Возвращает число непрерывных измерений, начиная с последнего, с такими же настройками, как у последнего.
    static int NumElementsWithSameSettings();
    /// Возвращает число непрерывных измерений, начиная с последнего, с текущими настройками прибора.
    static int NumElementsWithCurrentSettings();
    /// Возвращает количество сохранённых измерений.
    static int NumElementsInStorage();
    /// Получить данные. Счёт идёт с конца. Даннные будут сохранены по адресам ds, dataA, dataB..
    static bool GetDataFromEnd(int fromEnd, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    /// Получить указатели на сохранённые данные. Данные хранятся в ОЗУ.
    static bool GetDataFromEnd_RAM(int fromEnd, DataSettings **ds, uint16 **dataA, uint16 **dataB);
    /// Возвращет указатель, который указывает на данные в памяти внешнего ОЗУ. Доступ к данным осуществляется по полусловам
    static uint8 *GetData_RAM(Channel ch, int fromEnd);
    /// Получить усреднённые данные по нескольким измерениям
    static uint8 *GetAverageData(Channel ch);
    /// Возвращает указатель на данные, отстоящие на indexFromEnd oт последнего сохранённого
    static DataSettings *DataSettingsFromEnd(int indexFromEnd);
    /// Получить ограничивающую линию сигнала 0 - снизу, 1 - сверху
    static bool GetLimitation(Channel ch, uint8 *data, int direction);
    /// Сколько измерений может быть сохранено при такой же длине данных, как у последнего записанного
    static int NumberAvailableEntries();

    //////////////////////////////// Функции для поточечного вывода ////////////////////////////////////////

    /// Создаёт в хранилище место для помещения туда точек.Данные этого фрейма можно получить GetDataFromEnd_RAM(0...)
    static void NewFrameP2P(DataSettings *dss);
    /// Поместить очередные считанные точки в специально выделенный для них фрейм
    static void AddPointsP2P(uint16 dataA, uint16 dataB);
    /// \brief Получить данные фрейма поточечного вывода. Последние полученные данные можно получить вызовом GetDataFromEnd_RAM(1...). 
    /// Возвращает общее количество точек (оно может быть больше, чем количество точек в канале. В этом случае в канале находятся последние 
    /// numPointsInChan точек.
    static int GetFrameP2P_RAM(DataSettings **ds, uint8 **dataA, uint8 **dataB);

private:
    static void CalculateAroundAverage(uint8 *dataA, uint8 *dataB, DataSettings *dss);

    static DataSettings* GetSettingsDataFromEnd(int fromEnd);

    static void CalculateLimits(uint8 *dataA, uint8 *dataB, DataSettings *dss);

    static void CalculateSums();
    /// Возвращает true, если настройки измерений с индексами elemFromEnd0 и elemFromEnd1 совпадают, и false в ином случае.
    static bool SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1);
};

////////////////////////////// Функции для самописца //////////////////////////////////////////////////

/** @}  @}
 */
