#pragma once 
#include "defines.h"
#include "Controls.h"


void    Panel_Init();
void    Panel_DeInit();
void    Panel_Update();
void    Panel_TransmitData(uint16 data);            // Передать даннные в мк панели управления.
void    Panel_Disable();                        // В отлюченном режиме панель лишь обновляет состояние переменной pressedButton, не выполняя больше никаких действий.
void    Panel_Enable();
PanelButton   Panel_WaitPressingButton();       // Ожидать нажатие клавиши.
bool    Panel_ProcessingCommandFromPIC(uint16 command);
uint16  Panel_NextData();
PanelButton  Panel_PressedButton();
