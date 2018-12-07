#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#include <ff.h>
#pragma clang diagnostic warning "-Wpadded"
#pragma clang diagnostic warning "-Wc++98-compat-pedantic"
#include "Display.h"
#include "Display/Painter.h"
#include "main.h"
#include "Display/Font/Font.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    TypeWelcomeScreen_Vague,
    TypeWelcomeScreen_Wave,
    TypeWelcomeScreen_VagueWave
} TypeWelcomeScreen;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawProgressBar(uint dT);
static void DrawBigMNIPI();
static int RandValue(int min, int max);
static void InitPoints();

typedef struct
{
    uint16 x;
    uint8  y;
    uint8  notUsed;
} Vector;

#ifdef MSVC
#define __attribute__(x)
#endif

static int numPoints = 0;
static Vector array[7000] __attribute__ ((section("CCM_DATA")));

static TypeWelcomeScreen typeScreen = TypeWelcomeScreen_Wave;

#define VAGUE (typeScreen == TypeWelcomeScreen_Vague)
#define WAVE (typeScreen == TypeWelcomeScreen_Wave)
#define ALL (typeScreen == TypeWelcomeScreen_VagueWave)
#define VAGUE_OR_ALL (VAGUE || ALL)
#define WAVE_OR_ALL (WAVE || ALL)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void InitHardware()
{
    GPIO_InitTypeDef isGPIO_ =
    {
        GPIO_PIN_11,
        GPIO_MODE_INPUT,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF0_MCO,
    };
    // Сигнал готовности дисплея  к приёму команды
    HAL_GPIO_Init(GPIOG, &isGPIO_);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Init()
{
    ms->display.value = 0.0f;
    ms->display.isRun = false;
    ms->display.timePrev = 0;
    ms->display.direction = 10.0f;

    Color::InitGlobalColors();

    for (int i = 0; i < 14; i++)
    {
        float red = i / 14.0f * 31.0f + 0.5f;
        float green = i / 14.0f * 63.0f + 0.5f;
        float blue = i / 14.0f * 31.0f + 0.5f;
        set.display.colors[i + 2] = (uint16)MAKE_COLOR((int)red, (int)green, (int)blue);
    }

    Painter::ResetFlash();

    InitHardware();

    Painter::LoadPalette();

    Painter::LoadFont(TypeFont_8);
    Painter::SetFont(TypeFont_8);

    InitPoints();
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawButton(int x, int y, char *text)
{
    int width = 25;
    int height = 20;
    Painter::DrawRectangle(x, y, width, height);
    Painter::DrawStringInCenterRect(x, y, width + 2, height - 1, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    ms->display.isRun = true;

    uint dT = TIME_MS - ms->display.timePrev;
    ms->display.timePrev = TIME_MS;

    Painter::BeginScene(Color::BLACK);

    Painter::SetColor(Color::WHITE);

    if (ms->state == State_Start || ms->state == State_Ok)
    {
        Painter::BeginScene(Color::BACK);
#ifdef S8_54
        Painter::DrawRectangle(0, 0, 319, 239, Color::FILL);
        DrawBigMNIPI();
        Painter::DrawStringInCenterRect(0, 180, 320, 20, "Для получения помощи нажмите и удерживайте кнопку ПОМОЩЬ", Color::WHITE);
        Painter::DrawStringInCenterRect(0, 205, 320, 20, "Отдел маркетинга: тел./факс. 8-017-237-23-40");
        Painter::DrawStringInCenterRect(0, 220, 320, 20, "Разработчики: e-mail: mnipi-24(@)tut.by, тел. 8-017-237-22-15");
#endif
        Painter::EndScene();
    }
    else if (ms->state == State_Mount)
    {
        DrawProgressBar(dT);
    }
    else if (ms->state == State_WrongFlash)
    {
        Painter::DrawStringInCenterRect(0, 0, 320, 200, "НЕ УДАЛОСЬ ПРОЧИТАТЬ ДИСК", Color::FLASH_10);
        Painter::DrawStringInCenterRect(0, 20, 320, 200, "УБЕДИТЕСЬ, ЧТО ФАЙЛОВАЯ СИСТЕМА FAT32", Color::WHITE);
    }
    else if (ms->state == State_RequestAction)
    {
        Painter::DrawStringInCenterRect(0, 0, 320, 200, "Обнаружено программное обеспечение");
        Painter::DrawStringInCenterRect(0, 20, 320, 200, "Установить его?");

        DrawButton(290, 55, "ДА");
        DrawButton(290, 195, "НЕТ");
    }
    else if (ms->state == State_Upgrade)
    {
        Painter::DrawStringInCenterRect(0, 0, 320, 190, "Подождите завершения");
        Painter::DrawStringInCenterRect(0, 0, 320, 220, "установки программного обеспечения");

        int height = 30;
        int fullWidth = 280;
        int width = (int)(fullWidth * ms->percentUpdate);

        Painter::FillRegion(20, 130, width, height);
        Painter::DrawRectangle(20, 130, fullWidth, height);
    }

    Painter::EndScene();
    ms->display.isRun = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawProgressBar(uint dT)
{
    const int WIDTH = 300;
    const int HEIGHT = 20;
    const int X = 10;
    const int Y = 200;
    
    float step = dT / ms->display.direction;

    ms->display.value += step;

    if (ms->display.direction > 0.0f && ms->display.value > WIDTH)
    {
        ms->display.direction = -ms->display.direction;
        ms->display.value -= step;
    }
    else if (ms->display.direction < 0.0f && ms->display.value < 0)
    {
        ms->display.direction = -ms->display.direction;
        ms->display.value -= step;
    }

    int dH = 15;
    int y0 = 50;

    Painter::DrawStringInCenterRect(X, y0, WIDTH, 10, "Обнаружен USB-диск.", Color::WHITE);
    Painter::DrawStringInCenterRect(X, y0 + dH, WIDTH, 10, "Идёт поиск программного обеспечения");
    Painter::DrawStringInCenterRect(X, y0 + 2 * dH, WIDTH, 10, "Подождите...");

    Painter::DrawRectangle(X, Y, WIDTH, HEIGHT);
    Painter::FillRegion(X, Y, (int)ms->display.value, HEIGHT);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::IsRun()
{
    return ms->display.isRun;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawBigMNIPI()
{
    static uint startTime = 0;
    static bool first = true;
    
    if(first)
    {
        first = false;
        startTime = TIME_MS;
    }

    uint time = TIME_MS - startTime;

    int numColor = (int)(time / (float)TIME_WAIT * 14.0f);
    Limitation(&numColor, 0, 13);

    Painter::SetColor((Color)((uint8)(numColor + 2)));

    float amplitude = 3.0f - (time / (TIME_WAIT / 2.0f)) * 3;
    LIMIT_BELOW(amplitude, 0.0f);
    float frequency = 0.05f;

    float radius = 5000.0f * (TIME_WAIT) / 3000.0f / time;
    LIMIT_BELOW(radius, 0);

    float shift[240];

    for (int i = 0; i < 240; i++)
    {
        shift[i] = WAVE_OR_ALL ? amplitude * sinf(frequency * time + i / 5.0f) : 0;
    }

    for (int i = 0; i < numPoints; i++)
    {
        int x = array[i].x + (VAGUE_OR_ALL ? RandValue((int)-radius, (int)radius) : 0) + (int)shift[array[i].y];
        int y = array[i].y + (VAGUE_OR_ALL ? RandValue((int)-radius, (int)radius) : 0);
        if (x > 0 && x < 319 && y > 0 && y < 239)
        {
            Painter::SetPoint(x, y);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------]
static int RandValue(int min, int max)
{
    int value = rand() % (max - min + 1);

    return value + min;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool ByteFontNotEmpty(int eChar, int byte)
{
    static const uint8 *bytes = 0;
    static int prevChar = -1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }
    return bytes[byte];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;      // WARN здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }
    return prevByte & (1 << bit);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int DrawBigCharInBuffer(int eX, int eY, int size, char symbol, uint8 buffer[320][240])
{
    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            int fullX = x + i;
                            int fullY = y + j;

                            if (fullX >= 0 && fullX < 320 && fullY >= 0 && fullY < 240)
                            {
                                buffer[fullX][fullY] = 1;
                            }
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawBigTextInBuffer(int eX, int eY, int size, const char* text, uint8 buffer[320][240])
{
    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 240; y++)
        {
            buffer[x][y] = 0;
        }
    }

    int numSymbols = (int)strlen(text);

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigCharInBuffer(x, eY, size, text[i], buffer);
        x += size;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void InitPoints()
{
    uint8 buffer[320][240];

    DrawBigTextInBuffer(31, 70, 9, "МНИПИ", buffer);

    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 240; y++)
        {
            if (buffer[x][y])
            {
                array[numPoints].x = (uint16)x;
                array[numPoints].y = (uint8)y;
                numPoints++;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::AddStringToIndicating(pString)
{

}
