#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "Display/Display.h"
#include "Data/DataStorage.h"


#define MAX_NUM_SAVED_WAVES 23  // \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                // ������� � ���� ����� ����� �����.

namespace FLASHmem
{
    // ���� onlyNonReset == true, ����������� ������ �������������� ���������.
    void LoadSettings();
    void SaveSettings();
    // ���� ������� ����, ��������������� ������� ������� ����� true.
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
    // ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
    int GetSerialNumber(char buffer[17]);
};
