#include "defines.h"
#include <usbh_core.h>
#include "CPU.h"
#include <stm32f4xx.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *)
{
    GPIO_InitTypeDef isGPIOB =
    {
        GPIO_PIN_7,         // NL1
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOD, &isGPIOB);

    GPIO_InitTypeDef isGPIOD =
    {
        GPIO_PIN_0 |        // D2
        GPIO_PIN_1 |        // D3
        GPIO_PIN_4 |        // NOE/RD
        GPIO_PIN_5 |        // NWE/WR
        GPIO_PIN_7 |        // NE1
        GPIO_PIN_8 |        // D13
        GPIO_PIN_9 |        // D14
        GPIO_PIN_10 |       // D15
        GPIO_PIN_11 |       // A16
        GPIO_PIN_12 |       // A17
        GPIO_PIN_13 |       // A18
        GPIO_PIN_14 |       // D0
        GPIO_PIN_15,        // D1
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOD, &isGPIOD);

    GPIO_InitTypeDef isGPIOE =
    {
        GPIO_PIN_3 |        // A19
        GPIO_PIN_4 |        // A20
        GPIO_PIN_7 |        // D4
        GPIO_PIN_8 |        // D5
        GPIO_PIN_9 |        // D6
        GPIO_PIN_10 |       // D7
        GPIO_PIN_11 |       // D8
        GPIO_PIN_12 |       // D9
        GPIO_PIN_13 |       // D10
        GPIO_PIN_14 |       // D11
        GPIO_PIN_15,        // D12
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOE, &isGPIOE);

    GPIO_InitTypeDef isGPIOF =
    {
        GPIO_PIN_0 |        // A0
        GPIO_PIN_1 |        // A1
        GPIO_PIN_2 |        // A2
        GPIO_PIN_3 |        // A3
        GPIO_PIN_4 |        // A4
        GPIO_PIN_5 |        // A5
        GPIO_PIN_12 |       // A6
        GPIO_PIN_13 |       // A7
        GPIO_PIN_14 |       // A8
        GPIO_PIN_15,        // A9
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOF, &isGPIOF);

    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_0 |        // A10
        GPIO_PIN_1 |        // A11
        GPIO_PIN_2 |        // A12
        GPIO_PIN_3 |        // A13
        GPIO_PIN_4 |        // A14
        GPIO_PIN_5,         // A15
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_FMC
    };
    HAL_GPIO_Init(GPIOG, &isGPIOG);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
// »нициализаци€ флешки
void HAL_HCD_MspInit(HCD_HandleTypeDef *)
{
    GPIO_InitTypeDef isGPIO =
    {
        0,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        0
    };

    /*
    104 - PA12 - D+
    103 - PA11 - D-
    101 - PA9  - VBUS
    */

    __SYSCFG_CLK_ENABLE();

    isGPIO.Speed = GPIO_SPEED_HIGH;
    isGPIO.Pin = GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12;
    isGPIO.Alternate = GPIO_AF10_OTG_FS;

    HAL_GPIO_Init(GPIOA, &isGPIO);

    HAL_NVIC_SetPriority(OTG_FS_IRQn, PRIORITY_FLASHDRIVE_OTG);

    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
// »нициализац€ VCP
void HAL_PCD_MspInit(PCD_HandleTypeDef *)
{
    __GPIOB_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    GPIO_InitTypeDef  isGPIO =
    {
        GPIO_PIN_14 | GPIO_PIN_15,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF12_OTG_HS_FS,
    };

    HAL_GPIO_Init(GPIOB, &isGPIO);

    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

    HAL_NVIC_SetPriority(OTG_HS_IRQn, PRIORITY_VCP_OTG);

    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
