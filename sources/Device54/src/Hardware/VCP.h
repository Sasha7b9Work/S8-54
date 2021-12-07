#pragma once
#include "defines.h"
#include "Hardware/CPU.h"


#define CABLE_USB_IS_CONNECTED            VCP::cableUSBisConnected


class VCP
{
public:

    static void Init();

    static void SendBufferSynch(const uint8 *data, int size);

    // ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendStringAsynch(char *format, ...);

    static void Flush();

    static USBD_HandleTypeDef handleUSBD;

    static PCD_HandleTypeDef handlePCD;

    static bool connectedToUSB;

    static bool cableUSBisConnected;

private:
    static bool PrevSendingComplete();

    static void SendDataAsynch(uint8 *data, int size);
};
