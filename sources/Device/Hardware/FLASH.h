#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "Display/Display.h"
#include "Data/DataStorage.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup FLASH
 *  @brief Работа с внутренним ППЗУ
 *  @{
 */

#define MAX_NUM_SAVED_WAVES 23  ///< \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                ///< вывести в одну линию внизу сетки.

class FlashEEPROM
{
public:
    void LoadSettings();  ///< Если onlyNonReset == true, загружаются только несбрасываемые настройки.
    void SaveSettings();
    
    void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);    ///< Если даннные есть, соответствующий элемент массива равен true.
    bool ExistData(int num);
    void SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    void DeleteData(int num);
    void DeleteAllData();
};

extern FlashEEPROM flash;

class FlashOTP
{
public:
    bool SaveSerialNumber(char *servialNumber);
    int GetSerialNumber(char buffer[17]);   ///< Возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось.
};

extern FlashOTP otp;

/** @} @}
 */
