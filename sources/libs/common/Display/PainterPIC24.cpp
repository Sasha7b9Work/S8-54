#include "Painter.h"
#include "Ethernet/Ethernet.h"
#include "Ethernet/TcpSocket.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "VCP/VCP.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool framesElapsed = false;

#define TRANSMIT_NEED_FOR_FIRST     (stateTransmit == StateTransmit_NeedForTransmitFirst)
#define TRANSMIT_NEED_FOR_SECOND    (stateTransmit == StateTransmit_NeedForTransmitSecond)
#define TRANSMIT_IS_FREE            (stateTransmit == StateTransmit_Free)
#define TRANSMIT_IN_PROCESS         (stateTransmit == StateTransmit_InProcess)


static enum StateTransmit
{
    StateTransmit_Free,
    StateTransmit_NeedForTransmitFirst,  ///< Это когда нужно передать первый кадр - передаются шрифты.
    StateTransmit_NeedForTransmitSecond, ///< Это когда нужно передать второй и последующий кадры - шрифты не передаются.
    StateTransmit_InProcess
} stateTransmit = StateTransmit_Free;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::BeginScene(Color color)
{
#ifdef OSCI
    if (TRANSMIT_NEED_FOR_FIRST || TRANSMIT_NEED_FOR_SECOND)
    {
        bool needForLoadFonts = TRANSMIT_NEED_FOR_FIRST;
        stateTransmit = StateTransmit_InProcess;
        if (needForLoadFonts)
        {
            LoadPalette();
            LoadFont(TypeFont_5);
#define dT 100
            Ethernet::Update(dT);            /// \todo Говнокод. Доработать метод посылки в TCPSocket

            LoadFont(TypeFont_8);
            Ethernet::Update(dT);

            LoadFont(TypeFont_UGO);
            Ethernet::Update(dT);

            LoadFont(TypeFont_UGO2);
            Ethernet::Update(dT);
        }
    }
#endif

    FillRegion(0, 0, 319, 239, color);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    if (!framesElapsed)
    {
        framesElapsed = true;
        return;
    }
    uint8 command[4] = {INVALIDATE};
    SendToDisplay(command, 4);
    SendToInterfaces(command, 1);
    if (TRANSMIT_IN_PROCESS)
    {
        VCP_FLUSH();
        stateTransmit = StateTransmit_Free;
    }

    RunDisplay();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendFrame(bool first)
{
    if (TRANSMIT_IS_FREE)
    {
        stateTransmit = (first ? StateTransmit_NeedForTransmitFirst : StateTransmit_NeedForTransmitSecond);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendToDisplay(uint8 *bytes, int numBytes)
{
    for (int i = 0; i < numBytes; i += 4)
    {
        while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
        {
        };

        Timer::PauseOnTicks(75);    /// \todo Здесь время ожидание увеличено по сравнению с С8-53 (там частота 120МГц, здесь - 180МГц)

        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
    }
}

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4100)
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendToInterfaces(uint8 * pointer, int size)
{
    if (TRANSMIT_IN_PROCESS)
    {
        VCP_SEND_DATA_SYNCH(pointer, size);
        SOCKET_SEND((const char *)pointer, size);
    }
}

#ifdef _WIN32
#pragma warning(pop)
#endif
