// 2024/01/05 15:03:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"


namespace Panel
{
    // Инициализация аппартаного обеспечения
    void Init();

    // Функцию нужно вызывать каждый кадр для обработки полученных в данном кадре событий клавиатуры
    void Update();

    // Включить/выключить лампочку одного из каналов
    void EnableLEDChannel(Channel ch, bool enable);

    // Включить/выключить лампочку УСТАНОВКА
    void EnableLEDRegSet(bool enable);

    // Включить/выключить лампочку СИНХР
    void EnableLEDTrig(bool enable);

    // Передача данных в МК клавиатуры
    void TransmitData(uint16 data);

    // Отлючить панель управления
    // Когда панель отключена, то происходит лишь обновление состояния переменной releasedButton
    void Disable();

    // Включить панель управления
    void Enable();

    // Обработать полученную команду
    bool ProcessingCommandFromPIC(uint16 command);

    // Возвращает следующие данные, подготовленные для отправки в панель, из буфера
    uint16 NextData();

    // Ожидание нажатия клавиши
    Key::E WaitPressingButton();

    // Отлкючает прибор, если была нажата кнопка ПИТАНИЕ
    void DisableIfNessessary();

    // Возвращает true, если button - функциональная клавиша [1..5]
    bool IsFunctionalButton(Key::E);

    void Long_Help();
};
