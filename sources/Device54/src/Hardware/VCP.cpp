#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "usbd_desc.h"
#include "Utils/Math.h"
#include <stdarg.h>



USBD_HandleTypeDef VCP::handleUSBD;
PCD_HandleTypeDef  VCP::handlePCD;
bool               VCP::cableUSBisConnected = false;
bool               VCP::connectedToUSB = false;



void VCP::Init()
{
    USBD_Init(&handleUSBD, &VCP_Desc, 0);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
} 


bool VCP::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    return pCDC->TxState == 0;
}


void VCP::SendDataAsynch(uint8 *buffer, int size)
{
#define SIZE_BUFFER 64
    static uint8 trBuf[SIZE_BUFFER];

    while (CONNECTED_TO_USB && size > 0)
    {
        while (!PrevSendingComplete()) { };

        if (size > SIZE_BUFFER)
        {
            memcpy(trBuf, buffer, SIZE_BUFFER);
            size -= SIZE_BUFFER;
            buffer += SIZE_BUFFER;

            USBD_CDC_SetTxBuffer(&handleUSBD, trBuf, SIZE_BUFFER);
            USBD_CDC_TransmitPacket(&handleUSBD);
        }
        else
        {
            memcpy(trBuf, buffer, (uint)size);

            USBD_CDC_SetTxBuffer(&handleUSBD, trBuf, (uint16)size);
            USBD_CDC_TransmitPacket(&handleUSBD);

            size = 0;
        }
    }
}


#define SIZE_BUFFER_VCP 256     /// \todo ���� ��������� ������ ������ 512, �� �� ��207 �����
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;


void VCP::Flush()
{
    if(CONNECTED_TO_USB)
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
}


void VCP::SendFormatStringAsynch(char *format, ...)
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
