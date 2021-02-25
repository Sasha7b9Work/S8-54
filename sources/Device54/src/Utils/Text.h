#pragma once
#include "Display/Colors.h"


class Char
{
public:
    Char(char s) : symbol(s) { }
    int Draw(int x, int y, Color color = Color::NUMBER);
    void Draw4InRect(int x, int y, Color color = Color::NUMBER);
    void Draw2Horizontal(int x, int y, Color color = Color::NUMBER);
    bool IsLetter();
    bool IsConsonant();
private:
    char symbol;
};


class BigChar
{
public:
    BigChar(char s, int _size) : symbol(s), size(_size) { }
    int Draw(int eX, int eY);
private:
    char symbol;
    int size;
};


class BigText
{
public:
    BigText(pString t, int s) : text(t), size(s) {}
    void Draw(int x, int y, Color color = Color::NUMBER);
private:
    pString text;
    int size;
};


struct TypeConversionString
{
    enum E
    {
        None,           // Преобразование строки не производится
        FirstUpper      // Первый символ - в вернем регистре, остальные - в нижнем
    };
};


class String // -V690
{
public:
    explicit String();
    String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void Set(TypeConversionString::E conv, const char *format, ...);

    char *c_str() const;

    int Draw(int x, int y, Color color = Color::NUMBER) const;
    
    int DrawInCenterRect(int x, int y, int width, int height, Color color = Color::NUMBER) const;
    
    int DrawInArea(int x, int y, int width, Color color = Color::NUMBER);
    
    void DrawRelativelyRight(int xRight, int y, Color color = Color::NUMBER);
    
    // Выввести слова "в столбик"
    void DrawInColumn(int x, int y, int width, int delta = 2);
    
    int DrawInColumnWithTransfers(const int left, const int top, const int width, const Color color = Color::NUMBER);
    
    int DrawInColumnWithTransfersDiffColors(const int left, const int top, const int width, const Color colorDif, const Color = Color::NUMBER);
    
    // Возвращает нижнюю координату прямоугольника
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBack, Color colorRect, Color colorText);
    
    // Пишет строку текста в центре области(x, y, width, height)цветом ColorText на прямоугольнике с шириной бордюра widthBorder цвета colorBackground
    void DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText, int widthBorder, Color colorBackground);
    
    int DrawInCenterRectAndBoundIt(int x, int y, int width, int height, Color colorBackground, Color colorFill);

    // Столько пикселей строка займёт в ширину при выводе на экран
    int Width() const;

    void Free();

    void Append(const char *str);

    void Append(const char *str, uint numSymbols);

    void Append(char symbol);
    // Удаляет numSymbols из начала строки
    void RemoveFromBegin(uint numSymbols);

    void RemoveFromEnd();

    // Размер строки в символах (без учёта заверщающего)
    int Size() const;

    // Возвращает колечество слов, разделённых пробелами
    int NumberWords() const;

    char &operator[](int i);

private:

    char *text;

    bool Allocate(uint size);

    void Conversion(TypeConversionString::E conv);

    int DrawSubString(int x, int y, pString t);

    int DrawSpaces(int x, int y, pString t, int *numSymbols);

    // Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если bool == false, то текст не влезет на экран 
    bool GetHeightTextWithTransfers(int left, int top, int right, int *height);

    // Если draw == false, то рисовать символ не надо, фунция используется только для вычислений
    int DrawPartWord(char *word, int x, int y, int xRight, bool draw);

    // Возвращает ширину, занимаемую словом до первого пробела или нуль-символа
    int WidthWithoutSpaces(const char *t) const;
};
