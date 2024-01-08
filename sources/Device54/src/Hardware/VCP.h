#pragma once
#include "defines.h"
#include "Hardware/CPU.h"


class VCP
{
public:

    static void Init();

    static void SendBufferSynch(const uint8 *data, int size);

    // ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendStringAsynch(char *format, ...);

    // ������ ��������� "��� ����"
    static void SendStringAsynchRAW(char *format, ...);

    static void Flush();

    static USBD_HandleTypeDef handleUSBD;

    static PCD_HandleTypeDef handlePCD;

    static bool isConnected;

    static bool cableIsConnected;

private:
    static bool PrevSendingComplete();

    static void SendDataAsynch(uint8 *data, int size);
};
