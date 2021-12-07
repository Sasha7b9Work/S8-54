#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "Display/Display.h"
#include "Data/DataStorage.h"




/** @addtogroup Hardware
 *  @{
 *  @defgroup FLASH
 *  @brief ������ � ���������� ����
 *  @{
 */

#define MAX_NUM_SAVED_WAVES 23  ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                ///< ������� � ���� ����� ����� �����.

class FLASHmem
{
public:
    /// ���� onlyNonReset == true, ����������� ������ �������������� ���������.
    static void LoadSettings();
    static void SaveSettings();
    /// ���� ������� ����, ��������������� ������� ������� ����� true.
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
    /// ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
    static int GetSerialNumber(char buffer[17]);
};

/** @} @}
 */
