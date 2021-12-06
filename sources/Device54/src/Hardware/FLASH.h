#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "Display/Display.h"
#include "Data/DataStorage.h"




/** @addtogroup Hardware
 *  @{
 *  @defgroup FLASH
 *  @brief Работа с внутренним ППЗУ
 *  @{
 */

#define MAX_NUM_SAVED_WAVES 23  ///< \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                ///< вывести в одну линию внизу сетки.

class FLASHmem
{
public:
    /// Если onlyNonReset == true, загружаются только несбрасываемые настройки.
    static void LoadSettings();
    static void SaveSettings();
    /// Если даннные есть, соответствующий элемент массива равен true.
    static void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);
    static bool ExistData(int num);
    static void SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    static bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    static void DeleteData(int num);
    static void DeleteAllData();
};

class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// Возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось.
    static int GetSerialNumber(char buffer[17]);
};

/** @} @}
 */
