#pragma once 
#include "defines.h"
#include "Panel/Controls.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Panel 
 *  @brief Работа с панелью управления
 *  @{
 */

class Panel
{
public:
    /// Инициализация аппартаного обеспечения
    static void Init();
    /// Функцию нужно вызывать каждый кадр для обработки полученных в данном кадре событий клавиатуры
    static void Update();
    /// Включить/выключить лампочку одного из каналов
    static void EnableLEDChannel(Channel ch, bool enable);
    /// Включить/выключить лампочку УСТАНОВКА
    static void EnableLEDRegSet(bool enable);
    /// Включить/выключить лампочку СИНХР
    static void EnableLEDTrig(bool enable);
    /// Передача данных в МК клавиатуры
    static void TransmitData(uint16 data);
    /// Отлючить панель управления
    /** Когда панель отключена, то происходит лишь обновление состояния переменной releasedButton */
    static void Disable();
    /// Включить панель управления
    static void Enable();
    /// Обработать полученную команду
    static bool ProcessingCommandFromPIC(uint16 command);
    /// Возвращает следующие данные, подготовленные для отправки в панель, из буфера
    static uint16 NextData();
    /// Ожидание нажатия клавиши
    static PanelButton WaitPressingButton();
    /// Отлкючает прибор, если была нажата кнопка ПИТАНИЕ
    static void DisableIfNessessary();
    /// Возвращает true, если button - функциональная клавиша [1..5]
    static bool IsFunctionalButton(PanelButton button);
};


/**  @}
 */
