#include "main.h"
#include "globals.h"
#include "SCPI/SCPI.h"
#include "Log.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"



static USBD_CDC_LineCodingTypeDef LineCoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

#define APP_RX_DATA_SIZE  32
static uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */


static int8_t CDC_Itf_Init     ();
static int8_t CDC_Itf_DeInit   ();
static int8_t CDC_Itf_Control  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Itf_Receive  (uint8_t* pbuf, uint32_t *Len);


USBD_CDC_ItfTypeDef USBD_CDC_fops = 
{
  CDC_Itf_Init,
  CDC_Itf_DeInit,
  CDC_Itf_Control,
  CDC_Itf_Receive
};



static void SetAttributeConnected()
{
    CABLE_USB_IS_CONNECTED = true;
    CONNECTED_TO_USB = false;
}



static int8_t CDC_Itf_Init()
{
    USBD_CDC_SetRxBuffer(&VCP::handleUSBD, UserRxBuffer);
    Timer::SetAndStartOnce(kUSB, SetAttributeConnected, 100);   /** \todo Задержка введена для того, чтобы не было ложных срабатываний в 
                                                                 usbd_conf.c:HAL_PCD_SetupStageCallback при определении подключения хоста */
    return (USBD_OK);
}



static int8_t CDC_Itf_DeInit()
{
    CABLE_USB_IS_CONNECTED = false;
    CONNECTED_TO_USB = false;

    return (USBD_OK);
}



static int8_t CDC_Itf_Control (uint8_t cmd, uint8_t* pbuf, uint16_t)
{ 
    switch (cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        /* Add your code here */
        break;
        
    case CDC_GET_ENCAPSULATED_RESPONSE:
        /* Add your code here */
        break;

    case CDC_SET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_GET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_CLEAR_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_SET_LINE_CODING:
        LineCoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
                                (pbuf[2] << 16) | (pbuf[3] << 24));
        LineCoding.format     = pbuf[4];
        LineCoding.paritytype = pbuf[5];
        LineCoding.datatype   = pbuf[6];
        break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = (uint8_t)(LineCoding.bitrate);
        pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
        pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
        pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
        pbuf[4] = LineCoding.format;
        pbuf[5] = LineCoding.paritytype;
        pbuf[6] = LineCoding.datatype;
        /* Add your code here */
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        /* Add your code here */
        break;

    case CDC_SEND_BREAK:
        /* Add your code here */
        break;    
    
    default:
        break;
    }
  
    return (USBD_OK);
}



static int8_t CDC_Itf_Receive(uint8 *buffer, uint *length)
{
    SCPI::AddNewData(buffer, *length);

    USBD_CDC_ReceivePacket(&VCP::handleUSBD);

    return (USBD_OK);
}
