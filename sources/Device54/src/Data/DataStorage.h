#pragma once
#include "Settings/SettingsTypes.h"
#include "Hardware/CPU.h"




/** @addtogroup FPGA
 *  @{
 *  @defgroup DataStorage Data Storage
 *  @{
 */

class Storage
{
public:
    // �������� ���� ���������� ���������.
    static void Clear();
    // �������� ��������� ������. ��� ���� ����������� ��������� ������� ��������� �������.
    static void AddData(uint8 *dataA, uint8 *dataB, DataSettings dss);
    // ���������� ����� ����������� ���������, ������� � ����������, � ������ �� �����������, ��� � ����������.
    static int NumElementsWithSameSettings();
    // ���������� ����� ����������� ���������, ������� � ����������, � �������� ����������� �������.
    static int NumElementsWithCurrentSettings();
    // ���������� ���������� ���������� ���������.
    static int NumElementsInStorage();
    // �������� ������. ���� ��� � �����. ������� ����� ��������� �� ������� ds, dataA, dataB..
    static bool GetDataFromEnd(int fromEnd, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    // �������� ��������� �� ���������� ������. ������ �������� � ���.
    static bool GetDataFromEnd_RAM(int fromEnd, DataSettings **ds, uint16 **dataA, uint16 **dataB);
    // ��������� ���������, ������� ��������� �� ������ � ������ �������� ���. ������ � ������ �������������� �� ����������
    static uint8 *GetData_RAM(Channel ch, int fromEnd);
    // �������� ���������� ������ �� ���������� ����������
    static uint8 *GetAverageData(Channel ch);
    // ���������� ��������� �� ������, ��������� �� indexFromEnd o� ���������� �����������
    static DataSettings *DataSettingsFromEnd(int indexFromEnd);
    // �������� �������������� ����� ������� 0 - �����, 1 - ������
    static bool GetLimitation(Channel ch, uint8 *data, int direction);
    // ������� ��������� ����� ���� ��������� ��� ����� �� ����� ������, ��� � ���������� �����������
    static int NumberAvailableEntries();

    //////////////////////////////// ������� ��� ����������� ������ ////////////////////////////////////////

    // ������ � ��������� ����� ��� ��������� ���� �����.������ ����� ������ ����� �������� GetDataFromEnd_RAM(0...)
    static void NewFrameP2P(DataSettings *dss);
    // ��������� ��������� ��������� ����� � ���������� ���������� ��� ��� �����
    static void AddPointsP2P(uint16 dataA, uint16 dataB);
    // \brief �������� ������ ������ ����������� ������. ��������� ���������� ������ ����� �������� ������� GetDataFromEnd_RAM(1...). 
    // ���������� ����� ���������� ����� (��� ����� ���� ������, ��� ���������� ����� � ������. � ���� ������ � ������ ��������� ��������� 
    // numPointsInChan �����.
    static int GetFrameP2P_RAM(DataSettings **ds, uint8 **dataA, uint8 **dataB);

private:
    static void CalculateAroundAverage(uint8 *dataA, uint8 *dataB, DataSettings *dss);

    static DataSettings* GetSettingsDataFromEnd(int fromEnd);

    static void CalculateLimits(uint8 *dataA, uint8 *dataB, DataSettings *dss);

    static void CalculateSums();
    // ���������� true, ���� ��������� ��������� � ��������� elemFromEnd0 � elemFromEnd1 ���������, � false � ���� ������.
    static bool SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1);
};

////////////////////////////// ������� ��� ��������� //////////////////////////////////////////////////

/** @}  @}
 */
