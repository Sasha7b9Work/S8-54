// 2024/01/05 15:03:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"


class Panel
{
friend class CPU;

private:

    // Инициализация аппартаного обеспечения
    static void Init();

public:

    // Функцию нужно вызывать каждый кадр для обработки полученных в данном кадре событий клавиатуры
    static void Update();

    // Включить/выключить лампочку одного из каналов
    static void EnableLEDChannel(Channel ch, bool enable);

    // Включить/выключить лампочку УСТАНОВКА
    static void EnableLEDRegSet(bool enable);

    // Включить/выключить лампочку СИНХР
    static void EnableLEDTrig(bool enable);

    // Передача данных в МК клавиатуры
    static void TransmitData(uint16 data);

    // Отлючить панель управления
    // Когда панель отключена, то происходит лишь обновление состояния переменной releasedButton
    static void Disable();

    // Включить панель управления
    static void Enable();

    // Обработать полученную команду
    static bool ProcessingCommandFromPIC(uint16 command);

    // Возвращает следующие данные, подготовленные для отправки в панель, из буфера
    static uint16 NextData();

    // Ожидание нажатия клавиши
    static Key::E WaitPressingButton();

    // Отлкючает прибор, если была нажата кнопка ПИТАНИЕ
    static void DisableIfNessessary();

    // Возвращает true, если button - функциональная клавиша [1..5]
    static bool IsFunctionalButton(Key::E);

    static void Long_Help();
};
