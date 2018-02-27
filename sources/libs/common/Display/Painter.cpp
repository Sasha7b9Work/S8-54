#include "defines.h"
#include "Log.h"
#include "Painter.h"
#include "Display/Colors.h"
#include "Ethernet/Ethernet.h"
#include "Ethernet/TcpSocket.h"
#include "FlashDrive/FileManager.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Hardware/VCP.h"
#include "stub.h"


/** @addtogroup Display
*   @{
*   @addtogroup Painter
*   @{
*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Color    Painter::currentColor = Color::NUMBER;
TypeFont Painter::currentTypeFont = TypeFont_None;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::LoadPalette()
{
    for (uint8 i = 0; i < Color::NUMBER.value; i++)
    {
        SetPalette(Color(i));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHPointLine(int y, int x0, int x1, float delta)
{
    for (int x = x0; x <= x1; x += (int)delta)
    {
        SetPoint(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVPointLine(int x, int y0, int y1, float delta)
{
    for (int y = y0; y <= y1; y += (int)delta)
    {
        SetPoint(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR_TRACE("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // Если линию нужно рисовать не с начала штриха
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            DrawHLine(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        DrawHLine(y, x, x + deltaFill - 1);
        x += (deltaFill + deltaEmpty);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
        LOG_ERROR_TRACE("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            DrawVLine(x, y0, y - 1);
        }
    }

    while (y < y1)
    {
        DrawVLine(x, y, y + deltaFill - 1);
        y += (deltaFill + deltaEmtpy);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade)
{
    FillRegion(x + thickness, y + thickness, width - thickness * 2, height - thickness * 2, normal);
    if (isPressed && !isShade)
    {
        for (int i = 0; i < thickness; i++)
        {
            DrawHLine(y + i, x + i, x + width - i, dark);
            DrawVLine(x + i, y + 1 + i, y + height - i);
            DrawVLine(x + width - i, y + 1 + i, y + height - i, bright);
            DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
    else
    {
        for (int i = 0; i < thickness; i++)
        {
            DrawHLine(y + i, x + i, x + width - i, bright);
            DrawVLine(x + i, y + 1 + i, y + height - i);
            DrawVLine(x + width - i, y + 1 + i, y + height - i, dark);
            DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 Painter::ReduceBrightness(uint16 colorValue, float newBrightness)
{
    int red = (int)(R_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(red, 0, 31);
    int green = (int)(G_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(green, 0, 63);
    int blue = (int)(B_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(blue, 0, 31);
    return MAKE_COLOR(red, green, blue);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Color Painter::GetColor()
{
    return currentColor;
}

#ifdef OSCI
//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 Read2points(int x, int y)
{
    while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
    {
    };
    Timer::PauseOnTicks(12);               /// \todo временно увеличено время ожидания - не читает флешку

    *ADDR_CDISPLAY = GET_PIXEL;
    *ADDR_CDISPLAY = (uint8)x;
    *ADDR_CDISPLAY = (uint8)(x >> 8);
    *ADDR_CDISPLAY = (uint8)y;

    while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
    {
    };
    /// \todo временно увеличено время ожидания - не читает флешку
    //Timer_PauseOnTicks(6);

    return *ADDR_CDISPLAY;
}
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Painter::SaveScreenToFlashDrive()
{
#ifdef OSCI
#pragma pack(1)
    typedef struct
    {
        char    type0;      // 0
        char    type1;      // 1
        uint    size;       // 2
        uint16  res1;       // 6
        uint16  res2;       // 8
        uint    offBits;    // 10
    } BITMAPFILEHEADER;
    // 14

    typedef struct
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    } BITMAPINFOHEADER;
    // 54
#pragma pack(4)

    BITMAPFILEHEADER bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 240 / 2,
        0,
        0,
        14 + 40 + 1024
    };

    StructForWrite structForWrite;
    char fileName[255];

    if (!FileManager::GetNameForNewFile(fileName))
    {
        return false;
    }

    FDrive::OpenNewFileForWrite(fileName, &structForWrite);

    FDrive::WriteToFile((uint8 *)(&bmFH), 14, &structForWrite);

    BITMAPINFOHEADER bmIH =
    {
        40, // size;
        320,// width;
        240,// height;
        1,  // planes;
        4,  // bitCount;
        0,  // compression;
        0,  // sizeImage;
        0,  // xPelsPerMeter;
        0,  // yPelsPerMeter;
        0,  // clrUsed;
        0   // clrImportant;
    };

    FDrive::WriteToFile((uint8 *)(&bmIH), 40, &structForWrite);

    uint8 buffer[320 * 3] = {0};

    typedef struct tagRGBQUAD
    {
        uint8    blue;
        uint8    green;
        uint8    red;
        uint8    rgbReserved;
    } RGBQUAD;

    RGBQUAD colorStruct;

    for (int i = 0; i < 16; i++)
    {
        uint16 color = COLOR(i);
        colorStruct.blue = (uint8)((float)B_FROM_COLOR(color) / 31.0f * 255.0f);
        colorStruct.green = (uint8)((float)G_FROM_COLOR(color) / 63.0f * 255.0f);
        colorStruct.red = (uint8)((float)R_FROM_COLOR(color) / 31.0f * 255.0f);
        colorStruct.rgbReserved = 0;
        ((RGBQUAD*)(buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(buffer, 256, &structForWrite);
    }

    FSMC_SET_MODE(ModeFSMC_Display);

    for (int y = 239; y >= 0; y--)
    {
        for (int x = 1; x < 320; x += 2)
        {
            uint8 color = Read2points(x, y);

            while (color != Read2points(x, y))      // Этот цикл нужен потому, что иногда цвета считываются неправильно - 
            {                                       // проверка, что повторное чтение читает тот же цвет.
                color = Read2points(x, y);
            }

            buffer[x / 2] = (uint8)(((color & 0x0f) << 4) + (color >> 4));
        }

        FDrive::WriteToFile(buffer, 160, &structForWrite);
    }

    FSMC_RESTORE_MODE();

    FDrive::CloseFile(&structForWrite);
#endif
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void Get4Bytes(uint8 bytes[4])
{
    while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET) {};
    bytes[0] = *ADDR_CDISPLAY;
    bytes[1] = *ADDR_CDISPLAY;
    bytes[2] = *ADDR_CDISPLAY;
    bytes[3] = *ADDR_CDISPLAY;
}
*/


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static Color GetColor(int x, int y)
{
    uint8 command[4];
    command[0] = GET_PIXEL;
    *((int16 *)(command + 1)) = (int16)x;
    *(command + 3) = (int8)y;
    SendToDisplay(command, 4);
    Get4Bytes(command);
    return (Color)(command[0] & 0x0f);
}
*/


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void Get8Points(int x, int y, uint8 buffer[4])
{
    uint8 command[4];
    command[0] = GET_PIXEL;
    *((int16 *)(command + 1)) = (int16)x;
    *(command + 3) = (int8)y;
    SendToDisplay(command, 4);
    Get4Bytes(buffer);
}
*/

/** @} @}
*/
