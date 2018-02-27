#include "Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char* namesButtons[] = 
{
    "B_Empty",
    "B_ChannelA",
    "B_Service",
    "B_ChannelB",
    "B_Display",
    "B_Time",
    "B_Memory",
    "B_Sinchro",
    "B_Start",
    "B_Cursors",
    "B_Measures",
    "B_Power",
    "B_Info",
    "B_Menu",
    "B_F1",
    "B_F2",
    "B_F3",
    "B_F4",
    "B_F5"
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* NameButton(PanelButton button) 
{
    return namesButtons[button];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PanelButton& operator++(PanelButton &button)
{
    button = static_cast<PanelButton>(static_cast<int>(button) + 1);
    return button;
}
