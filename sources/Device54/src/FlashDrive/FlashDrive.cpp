#include "FlashDrive.h"
#include "Globals.h"
#include "Log.h"
#include "Display/Display.h"
#include "Hardware/RTC.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "FileManager.h"
#include "Utils/Dictionary.h"
#include "usbh_diskio.h"
#include "Settings/SettingsService.h"
#include <ff_gen_drv.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NEED_MOUNT (bf.needToMoundFlash)

static struct BitFieldFlashDrive
{
    uint needToMoundFlash : 1;  ///< Установленное в 1 значение означает, что подсоединена флешка. Надо её монтировать.
} bf = {0};


USBH_HandleTypeDef hUSB_Host;
static FATFS USBDISKFatFs;
static char USBDISKPath[4];
static bool gFlashDriveIsConnected = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Устанавливает текущее время для файла nameFile
static void SetTimeForFile(char *nameFile);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
            NEED_MOUNT = 1;

            /*
            if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 1) != FR_OK)
            {
                display.ShowWarning(WrongFileSystem);
            }
            else
            {
                gFlashDriveIsConnected = true;
                FM_Init();
                Menu::ChangeStateFlashDrive();
            }
            */
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            f_mount(NULL, (TCHAR const*)"", 0);
            break;

        case HOST_USER_DISCONNECTION:
            gFlashDriveIsConnected = false;
            Menu::ChangeStateFlashDrive();
            break;

        default:
            break;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Mount()
{
    FileManager::Init();
    Menu::ChangeStateFlashDrive();
    if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
    {
        LOG_ERROR_TRACE("Не могу примонтировать диск");
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::IsConnected()
{
    return gFlashDriveIsConnected;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Init()
{
    if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK) 
    {
        USBH_StatusTypeDef res = USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
        res = USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
        res = USBH_Start(&hUSB_Host);
    }
    else
    {
        LOG_ERROR_TRACE("Can not %s", __FUNCTION__);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::Update()
{
    if (NEED_MOUNT)      // Если обнаружено физическое подключение внешнего диска
    {
        uint timeStart = gTimeMS;
        NEED_MOUNT = 0;

        Display::FuncOnWaitStart(DICT(DDetectFlashDrive), false);

        if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 1) != FR_OK)
        {
            Display::ShowWarning(WrongFileSystem);
        }
        else
        {
            gFlashDriveIsConnected = true;
            FileManager::Init();
            Menu::ChangeStateFlashDrive();
        }
        while (gTimeMS - timeStart < 3000)
        {
        };

        Display::FuncOnWaitStop();
    }
    else
    {
        USBH_Process(&hUSB_Host);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::AppendStringToFile(const char *)
{
    return false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void WriteToFile(FIL *file, char *string)
{
    //    uint bytesWritten;
    f_open(file, "list.txt", FA_OPEN_EXISTING);
    //f_write(file, string, strlen(string), (void *)&bytesWritten);
    f_puts(string, file);
    f_close(file);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;
    

    char nameDir[_MAX_LFN + 1];
    memcpy(nameDir, (void *)fullPath, strlen(fullPath));
    nameDir[strlen(fullPath)] = '\0';

    if (f_opendir(&dir, nameDir) == FR_OK)
    {
        int numReadingElements = 0;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(&dir, &fno) != FR_OK)
            {
                break;
            }
            if (fno.fname[0] == 0)
            {
                if(alreadyNull)
                {
                    break;
                }
                alreadyNull = true;
                continue;
            }
            numReadingElements++;
            if (fno.fname[0] != '.')
            {
                if (fno.fattrib & AM_DIR)
                {
                    (*numDirs)++;
                }
                else
                {
                    (*numFiles)++;
                }
            }
        }
        f_closedir(&dir);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s)
{
    memcpy(s->nameDir, (void *)fullPath, strlen(fullPath));
    s->nameDir[strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numDirs = 0;
        FILINFO *pFNO = &s->fno;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameDirOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numDir == numDirs && (pFNO->fattrib & AM_DIR))
            {
                strcpy(nameDirOut, pFNO->fname);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) && (pFNO->fname[0] != '.'))
            {
                numDirs++;
            }
        }
    }
    return false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::GetNextNameDir(char *nameDirOut, StructForReadDir *s)
{
    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(pDir, pFNO) != FR_OK)
        {
            *nameDirOut = '\0';
            f_closedir(pDir);
            return false;
        }
        else if (pFNO->fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            if (pFNO->fattrib & AM_DIR)
            {
                strcpy(nameDirOut, pFNO->fname);
                return true;
            }
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void FDrive::CloseCurrentDir(StructForReadDir *s)
{
    f_closedir(&s->dir);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s)
{
    memcpy(s->nameDir, (void *)fullPath, strlen(fullPath));
    s->nameDir[strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numFiles = 0;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameFileOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameFileOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numFile == numFiles && (pFNO->fattrib & AM_DIR) == 0)
            {
                strcpy(nameFileOut, pFNO->fname);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
            {
                numFiles++;
            }
        }
    }
    return false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::GetNextNameFile(char *nameFileOut, StructForReadDir *s)
{
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(&s->dir, &s->fno) != FR_OK)
        {
            *nameFileOut = '\0';
            f_closedir(&s->dir);
            return false;
        }
        if (s->fno.fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameFileOut = '\0';
                f_closedir(&s->dir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            if ((pFNO->fattrib & AM_DIR) == 0 && pFNO->fname[0] != '.')
            {
                strcpy(nameFileOut, pFNO->fname);
                return true;
            }
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite)
{
    if (f_open(&structForWrite->fileObj, fullPathToFile, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
        return false;
    }
    strcpy(structForWrite->name, (char *)fullPathToFile);
    structForWrite->sizeData = 0;
    return true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite)
{
    while (sizeData > 0)
    {
        int dataToCopy = sizeData;
        if (sizeData + structForWrite->sizeData > SIZE_FLASH_TEMP_BUFFER)
        {
            dataToCopy = SIZE_FLASH_TEMP_BUFFER - structForWrite->sizeData;
        }
        sizeData -= dataToCopy;
        memcpy(structForWrite->tempBuffer + structForWrite->sizeData, data, dataToCopy);
        data += dataToCopy;
        structForWrite->sizeData += dataToCopy;
        if (structForWrite->sizeData == SIZE_FLASH_TEMP_BUFFER)
        {
            uint wr = 0;
            if (f_write(&structForWrite->fileObj, structForWrite->tempBuffer, (uint)structForWrite->sizeData, &wr) != FR_OK || structForWrite->sizeData != (int)wr)
            {
                return false;
            }
            structForWrite->sizeData = 0;
        }
    }

    return true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool FDrive::CloseFile(StructForWrite *structForWrite)
{
    if (structForWrite->sizeData != 0)
    {
        uint wr = 0;
        if (f_write(&structForWrite->fileObj, structForWrite->tempBuffer, (uint)structForWrite->sizeData, &wr) != FR_OK || structForWrite->sizeData != (int)wr)
        {
            f_close(&structForWrite->fileObj);
            return false;
        }
    }
    f_close(&structForWrite->fileObj);

    SetTimeForFile(structForWrite->name);

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SetTimeForFile(char *name)
{
    FILINFO info;

    PackedTime time = RTClock::GetPackedTime();

    info.fdate = (WORD)(((time.year + 2000 - 1980) * 512) | time.month * 32 | time.day);
    info.ftime = (WORD)(time.hours * 2048 | time.minutes * 32 | time.seconds / 2);

    f_utime(name, &info);
}
