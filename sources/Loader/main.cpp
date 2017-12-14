/*
    Захрузчик располагается по адресу 0x08000000
    После сброса он проверяет наличие флешки.
        Если флешка есть:
            проверяет наличие файла S8-54.bin. Если таковой имеется, выводит сообщение "На USB-диске обнаружено новоое программное обеспечение. Установить?"
            Если нажато "Да":
                1. Стираются сектора:
                    5 - 0x08020000
                    6 - 0x08040000
                    7 - 0x08060000
                2. На их место записывается содержимое файла S8-54.bin
        Если флешку примонтировать не удалось:
            Вывести сообщение "Не удалось примонтировать флешку. Убедитесь, что на ней файловая система fat32"
    Далее выполняется переход по адресу, указанному в 0x0802004 (второе 32х-битное слово в таблице векторов, расположенной по адресу 0x0802000)
*/


#include "main.h"
#include "globals.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Panel/Panel.h"
#include "Hardware/Flash.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FILE_NAME "S8-54.bin"

typedef void(*pFunction)();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainStruct *ms;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Upgrade();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    ms = (MainStruct *)malloc(sizeof(MainStruct));
    ms->percentUpdate = 0.0f;
    
    HAL_Init();

    Hardware_Init();

    Settings_Load();

    Timer::PauseOnTime(250);
    /* 
    

    Display_Init();

    ms->state = State_Start;

    Timer::SetAndEnable(kTemp, Display_Update, 10);

    uint timeStart = gTimeMS;

    drive.Init();

    bool update = drive.Update();
    
    uint time = gTimeMS - timeStart;

    while (gTimeMS - timeStart < TIME_WAIT && !drive.Update())
    {
        update = drive.Update();
        time = gTimeMS - timeStart;
    }

    if ((ms->drive.connection && ms->drive.active == 0) ||  // Если флеша подключена, но в активное состояние почему-то не перешла
        (ms->drive.active && ms->state != State_Mount))     // или перешла в активное состояние, по почему-то не запустился процесс монтирования
    {
        free(ms);
        NVIC_SystemReset();
    }

    if (ms->state == State_Mount)                           // Это означает, что диск удачно примонтирован
    {
        if (drive.FileExist(FILE_NAME))                    // Если на диске обнаружена прошивка
        {
            ms->state = State_RequestAction;
            
            while (1)
            {
                PanelButton button = Panel_PressedButton();
                if (button == B_F1)
                {
                    ms->state = State_Upgrade;
                    Upgrade();
                    break;
                }
                else if (button == B_F5)
                {
                    ms->state = State_Ok;
                    break;
                }
            }
        }
        else
        {
            ms->state = State_NotFile;
        }
    }
    else if (ms->state == State_WrongFlash) // Диск не удалось примонтировать
    {
        Timer::PauseOnTime(5000);
    }

    ms->state = State_Ok;

    Panel_DeInit();

    Timer::Disable(kTemp);

    while (Display_IsRun())
    {
    }

    Display_Update();
    
    HAL_DeInit();

    free(ms);
    */
    
    __disable_irq();
    // Теперь переходим на основную программу
#ifndef WIN64
    pFunction JumpToApplication;

    JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));
    __set_MSP(*(__IO uint *)MAIN_PROGRAM_START_ADDRESS);
    __enable_irq();
    JumpToApplication();
#endif
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Upgrade()
{
#define sizeSector (1 * 1024)
    
    uint8 buffer[sizeSector];
    
    FLASH_Prepare();
    
    int size = drive.OpenFileForRead(FILE_NAME);
    int fullSize = size;
    uint address = ADDR_SECTOR_PROGRAM_0;

    while (size)
    {
        int readedBytes = drive.ReadFromFile(sizeSector, buffer);
        WriteData(address, buffer, readedBytes);
        size -= readedBytes;
        address += readedBytes;

        ms->percentUpdate = 1.0f - (float)size / fullSize;
    }
    
    drive.CloseOpenedFile();
}
