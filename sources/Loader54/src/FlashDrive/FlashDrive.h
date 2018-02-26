#pragma once
#include "defines.h"
#include "main.h"


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

    static HCD_HandleTypeDef handleHCD;
};
