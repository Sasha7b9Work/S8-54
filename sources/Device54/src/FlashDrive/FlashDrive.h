#pragma once
#include "defines.h"
#include <ff.h>



/** @defgroup FlashDrive
 *  @brief ������ � �������
 *  @{
 */

#define SIZE_FLASH_TEMP_BUFFER 512
typedef struct
{
    uint8   tempBuffer[SIZE_FLASH_TEMP_BUFFER];
    int     sizeData;
    FIL     fileObj;
    char    name[255];
    uint8   notUsed0;
} StructForWrite;

typedef struct
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
} StructForReadDir;

class FDrive
{
public:
    static void Init();
    
    static void Update();
    
    static void Mount();
    
    static bool IsConnected();
    
    static void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);
    
    static bool GetNameDir(const char *fuulPath, int numDir, char *nameDirOut, StructForReadDir *sfrd);
    
    static bool GetNextNameDir(char *nameDirOut, StructForReadDir *sfrd);
    
    static void CloseCurrentDir(StructForReadDir *sfrd);
    
    static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *sfrd);
    
    static bool GetNextNameFile(char *nameFileOut, StructForReadDir *sfrd);
    
    static bool OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite); // ������� ������� ���� ��� ������. ���� ����� ���� ��� ����������, ����� ���, ������� ����� ������� ����� � ������� ���
    
    static bool WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite);
    
    static bool CloseFile(StructForWrite *structForWrite);
    
    static bool AppendStringToFile(const char *string);
    
    static USBH_HandleTypeDef hUSB_Host;
};


#define FDRIVE_IS_CONNECTED (FDrive::IsConnected())

/** @}
 */
