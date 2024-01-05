// 2024/01/05 14:46:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "ffconf.h"


namespace Painter
{
    // ���������� � ������ ��������� ������� �����. ��������� ����� ������ color
    void BeginScene(Color color);

    // \brief ���������� ������� ���������� ����� ��������� � ���� �������, �� ��������� ����������� ��������� ������� EndScene(). 
    // ����� ��������, ���� ������� ��������� ����� ��������� ������ ������ ������ �������. ��������, ����� �������������� ����� �������� �� ������ � 
    // ������ ����������.
    void RunDisplay();

    // ���������� � ����� ��������� ������� �����. ��������� ���������� ������ �� �����
    void EndScene();

    // ������� ����������� �� ������� ���������� ����� USB ��� LAN. ���� first == true, �� ���������� �����
    void SendFrame(bool first);

    // ����� ������� �������. ����� ��� ����, ����� �������� ������ ��� ����������� �� �������� � ������
    void ResetFlash();

    // ���������� ���� ���������
    void SetColor(Color color);

    // ���������� ������� ���� ���������
    Color GetColor();

    // ������������� ����� �������� ��������. ����������� � ������� LoadPalette() ��� SetPalette()
    void SetColorValue(Color color, col_val value);

    // ��������� � ������� ��� �����
    void LoadPalette();

    // ��������� � ������� ��� ������� �����
    void SetPalette(Color color);

    // ���������� ����� ������� ������
    void SetPoint(int x, int y);

    // ���������� �������������� ����� �� x0 �� x1 ������� ����� ������ delta ��������
    void DrawHPointLine(int y, int x0, int x1, float delta);

    // ���������� ������������ ����� �� y0 �� y1 ������� ����� ������ delta ��������
    void DrawVPointLine(int x, int y0, int y1, float delta);

    // \brief ���������� numLines ������������ �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. �������������� ����������
    // ������ ����� ������ ����� ������������� ���������� �������� ������� x[]
    void DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color color = Color::NUMBER);

    // \brief ���������� numLines �������������� �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. ������������ ����������
    // ������ ����� ������ ����� ������������� ���������� �������� ������� y[]
    void DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color color = Color::NUMBER);

    // ���������� �������������� �����
    void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);

    // ���������� ������������ �����
    void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);

    // ���������� ������������ �����
    void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);

    // \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. 
    // dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);

    // ������ ����������� ������������ �����.
    void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade);

    // ���������� ������� �������.
    void SetBrightnessDisplay(int16 brightness);

    col_val ReduceBrightness(col_val colorValue, float newBrightness);

    // ���������� ������ ������������ �����. ����� �������� ���� �� ������. y0y1 - ������ ������������ ���������.
    void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);

    // modeLines - true - �������, false - �������.
    void DrawSignal(int x, uint8 data[281], bool modeLines);

    void DrawPicture(int x, int y, int width, int height, uint8 *address);

    bool SaveScreenToFlashDrive();

    void SendToDisplay(uint8 *bytes, int numBytes);

    void SendToInterfaces(uint8 *pointer, int size);

    void SetFont(TypeFont typeFont);

    //  ��������� ������ � �������
    void LoadFont(TypeFont typeFont);

    int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);

    // ������� ����� �� �������������� ����� colorBackgound
    int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);

    int DrawFormatText(int x, int y, char *format, ...);

    // ����� ������ � ������� x, y
    int DrawFormText(int x, int y, Color color, pString text, ...);

    int DrawTextWithLimitationC(int x, int y, const char *text, Color color, int limitX, int limitY, int limitWidth, int limitHeight);

    // ���������� ������ ���������� ��������������
    int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill);

    int DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color color);

    int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);

    // ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground
    void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder, 
                                             Color colorBackground);

    int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);

    void DrawTextInRect(int x, int y, int width, const char *text);

    void DrawTextRelativelyRight(int xRight, int y, const char *text, Color color = Color::NUMBER);

    void Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color color1, Color color2);

    void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    void Draw10SymbolsInRect(int x, int y, char eChar);

    // ����� ����� � ����������
    int DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

    void DrawBigText(int x, int y, int size, const char *text, Color color = Color::NUMBER);
    
    int DrawBigChar(int eX, int eY, int size, char symbol);
};


#define WRITE_BYTE(offset, value)   *(command + offset) = (uint8)value
#define WRITE_SHORT(offset, value)  *((uint16 *)(command + offset)) = (uint16)value
