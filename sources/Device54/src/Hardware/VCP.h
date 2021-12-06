#pragma once
#include "defines.h"
#include "Hardware/CPU.h"


#define VCP_FLUSH()                       VCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendDataAsynch(buffer, size)
#define CONNECTED_TO_USB                  VCP::connectedToUSB
#define CABLE_USB_IS_CONNECTED            VCP::cableUSBisConnected


class VCP
{
public:

    // Инициализация
    static void Init();

    static void SendDataAsynch(uint8 *data, int size);

    // Эта строка передаётся с завершающими символами \\r\\n
    static void SendFormatStringAsynch(char *format, ...);

    static void Flush();

    static USBD_HandleTypeDef handleUSBD;

    static PCD_HandleTypeDef handlePCD;

    static bool connectedToUSB;

    static bool cableUSBisConnected;

private:
    static bool PrevSendingComplete();
};
