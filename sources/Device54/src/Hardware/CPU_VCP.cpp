#include "Hardware/CPU.h"
#include "usbd_desc.h"
#include "Utils/Math.h"
#include <stdarg.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBD_HandleTypeDef CPU::VCP::handleUSBD;
PCD_HandleTypeDef  CPU::VCP::handlePCD;
bool               CPU::VCP::cableUSBisConnected = false;
bool               CPU::VCP::connectedToUSB = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::VCP::Init()
{
    USBD_Init(&handleUSBD, &VCP_Desc, 0);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
} 

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::VCP::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    return pCDC->TxState == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::VCP::SendDataAsynch(uint8 *buffer, int size)
{
#define SIZE_BUFFER 64
    static uint8 trBuf[SIZE_BUFFER];

    size = Min(size, SIZE_BUFFER);
    while (!PrevSendingComplete())  {};
    memcpy(trBuf, buffer, (uint)size);

    USBD_CDC_SetTxBuffer(&handleUSBD, trBuf, (uint16)size);
    USBD_CDC_TransmitPacket(&handleUSBD);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define SIZE_BUFFER_VCP 256     /// \todo если поставить размер буфера 512, то на ТЕ207 глюки
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::VCP::Flush()
{
    if (sizeBuffer)
    {
        USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
        while (pCDC->TxState == 1) {};
        USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, (uint16)sizeBuffer);
        USBD_CDC_TransmitPacket(&handleUSBD);
        while (pCDC->TxState == 1) {};
    }
    sizeBuffer = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::VCP::SendDataSynch(const uint8 *buffer, int size)
{
    if (CONNECTED_TO_USB)
    {
        USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    
        do 
        {
            if (sizeBuffer + size > SIZE_BUFFER_VCP)
            {
                int reqBytes = SIZE_BUFFER_VCP - sizeBuffer;
                LIMITATION(reqBytes, 0, size);
                while (pCDC->TxState == 1) {};
                memcpy(buffSend + sizeBuffer, (void *)buffer, (uint)reqBytes);
                USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, SIZE_BUFFER_VCP);
                USBD_CDC_TransmitPacket(&handleUSBD);
                size -= reqBytes;
                buffer += reqBytes;
                sizeBuffer = 0;
            }
            else
            {
                memcpy(buffSend + sizeBuffer, (void *)buffer, (uint)size);
                sizeBuffer += size;
                size = 0;
            }
        } while (size);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::VCP::SendStringAsynch(char *data)
{
    SendDataAsynch((uint8 *)data, (int)strlen(data));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::VCP::SendStringSynch(char *data)
{
    SendDataSynch((uint8 *)data, (int)strlen(data));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::VCP::SendFormatStringAsynch(char *format, ...)
{
    if (CONNECTED_TO_USB)
    {
        static char buffer[200];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        strcat(buffer, "\r\n");
        SendDataAsynch((uint8 *)buffer, (int)strlen(buffer));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::VCP::SendFormatStringSynch(char *format, ...)
{
    char buffer[200];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    strcat(buffer, "\r\n");
    SendDataSynch((uint8 *)buffer, (int)strlen(buffer));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::VCP::SendByte(uint8 byte)
{
    SendDataSynch(&byte, 1);
}
