#include "defines.h"
#include "Hardware/VCP.h"
#include "GUI/ConsoleSCPI.h"
#include "Utils/String.h"


PCD_HandleTypeDef  handlePCD;


bool VCP::connectedToUSB = false;
bool VCP::cableUSBisConnected = false;


void VCP::Init()
{

}


void VCP::SendDataAsynch(const uint8 *text, int)
{
    ConsoleSCPI::Self()->AddText(reinterpret_cast<const char *>(text));
}


void VCP::SendStringAsynch(const char *message)
{
    String text(">>> %s", message);
    ConsoleSCPI::Self()->AddText(text.c_str());
}
