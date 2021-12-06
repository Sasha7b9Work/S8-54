#pragma once
#include "defines.h"
#include "Hardware/CPU.h"


#define VCP_FLUSH()                       VCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendDataSynch(buffer, size)
#define CONNECTED_TO_USB                  VCP::connectedToUSB
#define CABLE_USB_IS_CONNECTED            VCP::cableUSBisConnected

class VCP
{
public:

    static void Init();

    static void SendDataSynch(const uint8 *data, int size);

    // Передаётся строка без завершающего нуля
    static void SendStringAsynch(char *data);

    static void SendStringAsynch(pchar data);

    // Эта строка передаётся с завершающими символами \\r\\n
    static void SendFormatStringAsynch(char *format, ...);

    static void Flush();

    static USBD_HandleTypeDef handleUSBD;

    static PCD_HandleTypeDef handlePCD;

    static bool connectedToUSB;

    static bool cableUSBisConnected;

private:
    static bool PrevSendingComplete();

    static void SendDataAsynch(uint8 *data, int size);
};
