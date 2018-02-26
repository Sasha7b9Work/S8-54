#pragma once
#include "defines.h"
#include "main.h"


class FDrive
{
    friend class CPU;
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
};
