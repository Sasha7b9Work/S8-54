#include "defines.h"
#include "Keyboard/Keyboard.h"
#include "SCPI/KeySCPI.h"


// :KEY:PRESS:
static pCHAR FuncKeyPress(pCHAR);
// :KEY:LONG:
static pCHAR FuncKeyLong(pCHAR);

static void HintKey(String *);


static pCHAR const keyNames[Key::Count + 1] =
{
    " NONE",       // None
    " F1",         // F1
    " F2",         // F2
    " F3",         // F3
    " F4",         // F4
    " 0",          // _0
    " 1",          // _1
    " 2",          // _2
    " 3",          // _3
    " 4",          // _4
    " 5",          // _5
    " 6",          // _6
    " 7",          // _7
    " 8",          // _8
    " 9",          // _9
    " DOT",        // Dot
    " MINUS",      // Minus
    " ESC",        // Esc
    " LEFT",       // Left
    " RIGHT",      // Right
    " CHANNELA",   // On1
    " CHANNELB",   // On2
    ""
};


const StructSCPI SCPI::key[] =
{
    SCPI_LEAF(":PRESS", FuncKeyPress, "Button press",            HintKey),
    SCPI_LEAF(":LONG",  FuncKeyLong,  "Press and hold a button", HintKey),
    SCPI_EMPTY()
};


static pCHAR FuncKeyPress(pCHAR buffer) //-V2506
{
    for(int i = 0; i < Key::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            Keyboard::AppendEvent(static_cast<Key::E>(i), Action::Down);
            Keyboard::AppendEvent(static_cast<Key::E>(i), Action::Up);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static void HintKey(String *message)
{
    SCPI::ProcessHint(message, keyNames);
}


static pCHAR FuncKeyLong(pCHAR buffer) //-V2506
{
    for(int i = 0; i < Key::Count; i++)
    {
        const char *end = SCPI::BeginWith(buffer, keyNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            Keyboard::AppendEvent(static_cast<Key::E>(i), Action::Down);
            Keyboard::AppendEvent(static_cast<Key::E>(i), Action::Long);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static pCHAR const regNames[Key::Count + 1] =
{
    " LEFT",   // Поворот вправо
    " RIGHT",  // Поворот влево
    " PRESS",  // Нажатие
    ""
};


pCHAR SCPI::FuncReg(pCHAR buffer) //-V2506
{
    for (int i = 0; i < 3; i++)
    {
        const char *end = SCPI::BeginWith(buffer, regNames[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            Keyboard::AppendEvent(static_cast<Key::E>(i + Key::RotateLeft), Action::Down);
            Keyboard::AppendEvent(static_cast<Key::E>(i + Key::RotateLeft), Action::Up);

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


void SCPI::HintReg(String *)
{

}
