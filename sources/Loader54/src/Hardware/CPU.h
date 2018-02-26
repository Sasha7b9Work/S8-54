#pragma once
#include "Hardware/stm32/437/stm437.h"
#include <usbh_core.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PRIORITY_SYS_TICK           0, 0
#define PRIORITY_FPGA_ADC           1, 0
#define PRIORITY_FPGA_P2P           1, 1
#define PRIORITY_TIMER_TIM6         2, 0
#define PRIORITY_PANEL_EXTI9_5      3, 0
#define PRIORITY_PANEL_SPI1         4, 0
#define PRIORITY_SOUND_DMA1_STREAM5 5, 0
#define PRIORITY_FLASHDRIVE_OTG     6, 0
#define PRIORITY_VCP_OTG            7, 0
#define PRIORITY_RAM_DMA2_STREAM0   8, 0
    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ADDR_BANK           0x60000000

#define ADDR_CDISPLAY               ((uint8 *)(ADDR_BANK + 0x080000))    // ����� 8-������ ��������������� �������������� �����


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM437
{
public:
    static void Config();

    //----------------------------------------------------------------------------------------------------------------------------------------------------
    class FDrive
    {
    public:
        static void Init();

        static bool Update();

        static bool FileExist(char *fileName);

        static int OpenFileForRead(char *fileName);
        /// ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
        static int ReadFromFile(int numBytes, uint8 *buffer);

        static void CloseOpenedFile();

        static void HCD_IRQHandler();

        class LL_
        {
        public:
            static void InitHCD(USBH_HandleTypeDef *phost);

            static void SetToggle(uint8 pipe, uint8 toggle);

            static uint8 GetToggle(uint8 pipe);
        };

        static HCD_HandleTypeDef handleHCD;

        static USBH_HandleTypeDef  handleUSBH;
    };

private:
    static void EnablePeriphery();

    static void InitHardware();

    static void InitFSMC();
};

