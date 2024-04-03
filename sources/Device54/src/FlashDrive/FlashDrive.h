// 2024/01/05 21:19:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include <ff.h>



struct StructForWrite
{
    static const int SIZE_FLASH_TEMP_BUFFER = 512;
    uint8   tempBuffer[SIZE_FLASH_TEMP_BUFFER];
    int     sizeData;
    FIL     fileObj;
    char    name[255];
    uint8   notUsed0;
};

struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
};

namespace FDrive
{
    void Init();

    void Update();

    void Mount();

    bool IsConnected();

    void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);

    bool GetNameDir(const char *fuulPath, int numDir, char *nameDirOut, StructForReadDir *sfrd);

    bool GetNextNameDir(char *nameDirOut, StructForReadDir *sfrd);

    void CloseCurrentDir(StructForReadDir *sfrd);

    bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *sfrd);

    bool GetNextNameFile(char *nameFileOut, StructForReadDir *sfrd);

    bool OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite); // Функция создаст файл для записи. Если такой файл уже существует, сотрёт его, заменит новым нулевой длины и откроет его

    bool WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite);

    bool CloseFile(StructForWrite *structForWrite);

    bool AppendStringToFile(const char *string);

    extern USBH_HandleTypeDef hUSB_Host;
};


#define FDRIVE_IS_CONNECTED (FDrive::IsConnected())
