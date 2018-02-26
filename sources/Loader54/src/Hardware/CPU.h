#pragma once
#include "Hardware/stm32/437/stm437.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ADDR_BANK           0x60000000

#define ADDR_CDISPLAY               ((uint8 *)(ADDR_BANK + 0x080000))    // Здесь 8-битный низкоскоростной мультиплексный режим


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
        /// Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
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

