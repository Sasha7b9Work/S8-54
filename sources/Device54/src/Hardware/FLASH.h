#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "Display/Display.h"
#include "Data/DataStorage.h"


#define MAX_NUM_SAVED_WAVES 23  // \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                // вывести в одну линию внизу сетки.

namespace FLASHmem
{
    // Если onlyNonReset == true, загружаются только несбрасываемые настройки.
    void LoadSettings();
    void SaveSettings();
    // Если даннные есть, соответствующий элемент массива равен true.
    void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);
    bool ExistData(int num);
    void SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    void DeleteData(int num);
    void DeleteAllData();
};

namespace OTPmem
{
    bool SaveSerialNumber(char *servialNumber);
    // Возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось.
    int GetSerialNumber(char buffer[17]);
};
