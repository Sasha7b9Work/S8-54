#pragma once 
#include "defines.h"
#include "Controls.h"


class Panel
{
public:
    static void Init();
    static void DeInit();
    static void Update();
    /// Передать даннные в мк панели управления.
    static void TransmitData(uint16 data);
    /// В отлюченном режиме панель лишь обновляет состояние переменной pressedButton, не выполняя больше никаких действий.
    static void Disable();
    static void Enable();
    /// Ожидать нажатие клавиши.
    static PanelButton WaitPressingButton();
    static bool ProcessingCommandFromPIC(uint16 command);
    static uint16 NextData();
    static PanelButton PressedButton();
};
