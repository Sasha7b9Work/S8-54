#include "defines.h"

#ifdef STM32F437xx
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_hcd.h>
#include <stm32f437xx.h>
#endif

#ifdef STM32F746xx
#include <stm32f7xx_hal.h>
#include <stm32f7xx_hal_hcd.h>
#include <stm32f746xx.h>
#endif

#include <usbh_core.h>

#include "Hardware/CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                       LL Driver Callbacks (HCD -> USB Host Library)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_IncTimer (static_cast<USBH_HandleTypeDef *>(hhcd->pData));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Connect(static_cast<USBH_HandleTypeDef *>(hhcd->pData));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Disconnect(static_cast<USBH_HandleTypeDef *>(hhcd->pData));
} 

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *, uint8_t, HCD_URBStateTypeDef)
{
    /* To be used with OS to sync URB state with the global state machine */
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                       LL Driver Interface (USB Host Library --> HCD)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{  
    CPU::FDrive::LL_::InitHCD(phost);
  
    return USBH_OK;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
    HAL_HCD_DeInit(static_cast<HCD_HandleTypeDef *>(phost->pData));
    return USBH_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Start(static_cast<HCD_HandleTypeDef *>(phost->pData));
    return USBH_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Stop(static_cast<HCD_HandleTypeDef *>(phost->pData));
    return USBH_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
    USBH_SpeedTypeDef speed = USBH_SPEED_FULL;
      
    switch (HAL_HCD_GetCurrentSpeed(static_cast<HCD_HandleTypeDef *>(phost->pData)))
    {
    case 0: 
        speed = USBH_SPEED_HIGH;
        break;

    case 1: 
        speed = USBH_SPEED_FULL;    
        break;
      
    case 2: 
        speed = USBH_SPEED_LOW;   
        break;
      
     default:  
        speed = USBH_SPEED_FULL;    
        break;    
    }
    return speed;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
USBH_StatusTypeDef USBH_LL_ResetPort (USBH_HandleTypeDef *phost) 
{
    HAL_HCD_ResetPort(static_cast<HCD_HandleTypeDef *>(phost->pData));
    return USBH_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Returns the last transfered packet size.
  * @param  phost: Host handle
  * @param  pipe: Pipe index   
  * @retval Packet Size */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)  
{
    return HAL_HCD_HC_GetXferCount(static_cast<HCD_HandleTypeDef *>(phost->pData), pipe);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Opens a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  epnum: Endpoint Number
  * @param  dev_address: Device USB address
  * @param  speed: Device Speed 
  * @param  ep_type: Endpoint Type
  * @param  mps: Endpoint Max Packet Size                 
  * @retval USBH Status  */
USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t epnum, uint8_t dev_address, uint8_t speed, uint8_t ep_type,
                                    uint16_t mps)
{
    HAL_HCD_HC_Init(static_cast<HCD_HandleTypeDef *>(phost->pData), pipe, epnum, dev_address, speed, ep_type, mps);
    return USBH_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Closes a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index               
  * @retval USBH Status */
USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)   
{
    HAL_HCD_HC_Halt(static_cast<HCD_HandleTypeDef *>(phost->pData), pipe);
    return USBH_OK;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Submits a new URB to the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index    
  *          This parameter can be a value from 1 to 15
  * @param  direction: Channel number
  *          This parameter can be one of these values:
  *           0: Output 
  *           1: Input
  * @param  ep_type: Endpoint Type
  *          This parameter can be one of these values:
  *            @arg EP_TYPE_CTRL: Control type
  *            @arg EP_TYPE_ISOC: Isochrounous type
  *            @arg EP_TYPE_BULK: Bulk type
  *            @arg EP_TYPE_INTR: Interrupt type
  * @param  token: Endpoint Type
  *          This parameter can be one of these values:
  *            @arg 0: PID_SETUP
  *            @arg 1: PID_DATA
  * @param  pbuff: pointer to URB data
  * @param  length: length of URB data
  * @param  do_ping: activate do ping protocol (for high speed only)
  *          This parameter can be one of these values:
  *           0: do ping inactive 
  *           1: do ping active 
  * @retval USBH Status  */
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t direction, uint8_t ep_type, uint8_t token, uint8_t* pbuff, 
                                     uint16_t length, uint8_t do_ping) 
{
    HAL_HCD_HC_SubmitRequest(static_cast<HCD_HandleTypeDef *>(phost->pData), pipe, direction, ep_type, token, pbuff, length, do_ping);
    return USBH_OK;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Gets a URB state from the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *          This parameter can be a value from 1 to 15
  * @retval URB state
  *          This parameter can be one of these values:
  *            @arg URB_IDLE
  *            @arg URB_DONE
  *            @arg URB_NOTREADY
  *            @arg URB_NYET 
  *            @arg URB_ERROR  
  *            @arg URB_STALL   */
USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe) 
{
    return static_cast<USBH_URBStateTypeDef>(HAL_HCD_HC_GetURBState (static_cast<HCD_HandleTypeDef *>(phost->pData), pipe));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Drives VBUS.
  * @param  phost: Host handle
  * @param  state: VBUS state
  *          This parameter can be one of these values:
  *           0: VBUS Active 
  *           1: VBUS Inactive
  * @retval USBH Status  */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *, uint8_t)
{
    HAL_Delay(200);
    return USBH_OK;  
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Sets toggle for a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index   
  * @param  toggle: toggle (0/1)
  * @retval USBH Status  */
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *, uint8_t pipe, uint8_t toggle)   
{
    CPU::FDrive::LL_::SetToggle(pipe, toggle);
    return USBH_OK; 
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Returns the current toggle of a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval toggle (0/1)  */
uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *, uint8_t pipe)
{
    return CPU::FDrive::LL_::GetToggle(pipe);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/** @brief  Delay routine for the USB Host Library
  * @param  Delay: Delay in ms
  * @retval None  */
void USBH_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);  
}

#if (USBH_DEBUG_LEVEL > 1)

/*
void USBH_UsrLog(char *format, ...) {
}

void USBH_ErrLog(char *format, ...) {
}
void USBH_DbgLog(char *format, ...) {
}
*/
#endif
