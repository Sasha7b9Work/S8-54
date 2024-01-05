#pragma once


// Идентификаторы кнопок.

struct Key
{
    enum E
    {
        Empty = 0x00,     // кнопка не нажата
        ChannelA = 0x01,     // КАНАЛ 1
        Service = 0x02,     // СЕРВИС
        ChannelB = 0x03,     // КАНАЛ 2
        Display = 0x04,     // ДИСПЛЕЙ
        Time = 0x05,     // РАЗВ
        Memory = 0x06,     // ПАМЯТЬ
        Trig = 0x07,     // СИНХР
        Start = 0x08,     // ПУСК/СТАРТ
        Cursors = 0x09,     // КУРСОРЫ
        Measures = 0x0a,     // ИЗМЕР
        Power = 0x0b,     // ПИТАНИЕ
        Help = 0x0c,     // ПОМОЩЬ
        Menu = 0x0d,     // МЕНЮ
        F1 = 0x0e,     // 1
        F2 = 0x0f,     // 2
        F3 = 0x10,     // 3
        F4 = 0x11,     // 4
        F5 = 0x12,     // 5
        Count            // общее количество кнопок
    };
};

// Идентификаторы регуляторов.
typedef enum
{                           //               нажатие - отпускание
    R_Empty         = 0x00,
    R_RangeA        = 0x14,     // ВОЛЬТ/ДЕЛ кан.A  0x9c - 0x1c
    R_RShiftA       = 0x15,     //                  0x9d - 0x1d
    R_RangeB        = 0x16,     // ВОЛЬТ/ДЕЛ кан.B  0x9e - 0x1e
    R_RShiftB       = 0x17,     //                  0x9f - 0x1f
    R_TBase         = 0x18,     // ВРЕМЯ/ДЕЛ        0xa0 - 0x20
    R_TShift        = 0x19,     //                  0xa1 - 0x21
    R_TrigLev       = 0x1a,     // УРОВЕНЬ          0xa2 - 0x22
    R_Set           = 0x1b,     // УСТАНОВКА        0xa3 - 0x23
    R_Range1_BTN    = 0x1c,     // Плавное изменение масштаба по напряжению
    R_RShiftA_BTN   = 0x1d,     // Установка нуля
    R_RangeB_BTN    = 0x1e,     // Плавное изменение масштаба по наряжению
    R_RShiftB_BTN   = 0x1f,     // Установка нуля
    R_TBase_BTN     = 0x20,     // Включение режима растяжки
    R_TShift_BTN    = 0x21,     // Установка нуля
    R_TrigLev_BTN   = 0x22,     // Установка нуля синхронизации или автоматической синхронизации
    R_Set_BTN       = 0x23,
    B_NumRegulators
} PanelRegulator;

// Направления вращения регуляторов.
typedef enum
{
    Direction_Left,     // вращение регулятора влево
    Direction_Right,    // вращение регулятора вправо
    Direction_None      // вращение отсутствует
} RegulatorDirection;

/// Команды, принимаемые от панели
typedef enum
{
    C_None = 0,
    C_Reset =  0x24     ///< Команда сброса. Формируется одновременным нажатием нескольких кнопок на клавиатуре (КУРСОРЫ+ДИСПЛЕЙ+ПАМЯТЬ+F4)
} PanelCommand;

#define NUM_REGULATORS (R_Set - R_RangeA + 1)


const char *NameButton(Key::E);
const char *NameRegulator(PanelRegulator regulator);
