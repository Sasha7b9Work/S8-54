#pragma once
#include "defines.h"
#include "usbd_cdc_interface.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup VCP
 *  @brief Virtual Com Port
 *  @{
 */
 
#define VCP_FLUSH()                       VCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendDataSynch(buffer, size)

class VCP
{
public:
    /// Инициализация
    static void Init();

    static void SendDataAsynch(uint8 *data, int size);

    static void SendDataSynch(const uint8 *data, int size);
    /// Передаётся строка без завершающего нуля
    static void SendStringAsynch(char *data);
    /// Передаётся строка без завершающего нуля
    static void SendStringSynch(char *data);
    /// Эта строка передаётся с завершающими символами \\r\\n
    static void SendFormatStringAsynch(char *format, ...);
    /// Эта строка передаётся с завершающими символами \\r\\n
    static void SendFormatStringSynch(char *format, ...);

    static void SendByte(uint8 data);

    static void Flush();

    static USBD_HandleTypeDef handleUSBD;
    static PCD_HandleTypeDef handlePCD;

private:
    static bool PrevSendingComplete();
};


/** @}
 */
