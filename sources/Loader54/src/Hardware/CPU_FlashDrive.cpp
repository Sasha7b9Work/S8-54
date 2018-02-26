#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#include <ctype.h>
#include <usbh_def.h>
#include <ff_gen_drv.h>
#include <usbh_diskio.h>
#include <usbh_core.h>
#include <usbh_msc.h>
#pragma clang diagnostic warning "-Wpadded"
#pragma clang diagnostic warning "-Wc++98-compat-pedantic"
#include "defines.h"
#include "CPU.h"
#include "ffconf.h"
#include "main.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    char nameDir[_MAX_LFN + 1];
    char lfn[(_MAX_LFN + 1)];
    FILINFO fno;
    DIR dir;
} StructForReadDir;

HCD_HandleTypeDef CPU::FDrive::handleHCD;

USBH_HandleTypeDef CPU::FDrive::handleUSBH;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s);
static bool GetNextNameFile(char *nameFileOut, StructForReadDir *s);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8 id);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::FDrive::Init()
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

    ms->drive.state = StateDisk_Idle;
    ms->drive.connection = 0;
    ms->drive.active = 0;

    if (FATFS_LinkDriver(&USBH_Driver, ms->drive.USBDISKPath) == FR_OK)
    {
        USBH_StatusTypeDef res = USBH_Init(&handleUSBH, USBH_UserProcess, 0);
        res = USBH_RegisterClass(&handleUSBH, USBH_MSC_CLASS);
        res = USBH_Start(&handleUSBH);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
            ms->drive.active++;
            ms->drive.state = StateDisk_Start;
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            ms->drive.connection++;
            ms->state = State_Mount;
            f_mount(NULL, (TCHAR const*)"", 0);
            break;

        case HOST_USER_DISCONNECTION:
            break;

        default:
            break;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::FDrive::Update()
{
    USBH_Process(&handleUSBH);
    if (ms->drive.state == StateDisk_Start)
    {
        if (f_mount(&(ms->drive.USBDISKFatFS), (TCHAR const*)ms->drive.USBDISKPath, 0) == FR_OK)
        {
            return true;
        }
        else
        {
            ms->state = State_WrongFlash;
            return false;
        }
    }
    return false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ToLower(char *str)
{
    while (*str)
    {
        *str = (char)tolower(*str);
        str++;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::FDrive::FileExist(char *fileName)
{
    char nameFile[255];
    char file[255];
    strcpy(file, fileName);
    ToLower(file);
    StructForReadDir strd;

    if (GetNameFile("\\", 0, nameFile, &strd))
    {
        ToLower(nameFile);
        if (strcmp(file, nameFile) == 0)
        {
            return true;
        }
        while (GetNextNameFile(nameFile, &strd))
        {
            ToLower(nameFile);
            if (strcmp(file, nameFile) == 0)
            {
                return true;
            }
        }
    }

    return false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s)
{
    memcpy((uint8 *)s->nameDir, (uint8 *)fullPath, strlen(fullPath));
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
static bool GetNextNameFile(char *nameFileOut, StructForReadDir *s)
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
int CPU::FDrive::OpenFileForRead(char *fileName)
{
    if (f_open(&ms->drive.file, fileName, FA_READ) == FR_OK)
    {
        return (int)f_size(&ms->drive.file);
    }
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int CPU::FDrive::ReadFromFile(int numBytes, uint8 *buffer)
{
    uint readed = 0;
    if (f_read(&ms->drive.file, buffer, (UINT)numBytes, &readed) == FR_OK)
    {
        return (int)readed;
    }
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::FDrive::CloseOpenedFile()
{
    f_close(&ms->drive.file);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::FDrive::LL_::InitHCD(USBH_HandleTypeDef *phost)
{
    /* Set the LL driver parameters */
    FDrive::handleHCD.Instance = USB_OTG_FS;
    FDrive::handleHCD.Init.speed = HCD_SPEED_FULL;
    FDrive::handleHCD.Init.Host_channels = 11;
    FDrive::handleHCD.Init.dma_enable = 0;
    FDrive::handleHCD.Init.low_power_enable = 0;
    FDrive::handleHCD.Init.phy_itface = HCD_PHY_EMBEDDED;
    FDrive::handleHCD.Init.Sof_enable = 0;
    FDrive::handleHCD.Init.vbus_sensing_enable = 0;
    FDrive::handleHCD.Init.use_external_vbus = 0;

    /* Link the driver to the stack */
    FDrive::handleHCD.pData = phost;
    phost->pData = &FDrive::handleHCD;
    /* Initialize the LL driver */
    HAL_HCD_Init(&FDrive::handleHCD);

    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&FDrive::handleHCD));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::FDrive::LL_::SetToggle(uint8 pipe, uint8 toggle)
{
    if (FDrive::handleHCD.hc[pipe].ep_is_in)
    {
        FDrive::handleHCD.hc[pipe].toggle_in = toggle;
    }
    else
    {
        FDrive::handleHCD.hc[pipe].toggle_out = toggle;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 CPU::FDrive::LL_::GetToggle(uint8 pipe)
{
    uint8 toggle = 0;

    if (FDrive::handleHCD.hc[pipe].ep_is_in)
    {
        toggle = FDrive::handleHCD.hc[pipe].toggle_in;
    }
    else
    {
        toggle = FDrive::handleHCD.hc[pipe].toggle_out;
    }
    return toggle;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::FDrive::HCD_IRQHandler()
{

}
