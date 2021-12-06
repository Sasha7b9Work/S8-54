#include "defines.h"
#include "SCPI.h"
#include "commonSCPI.h"
#include <usbd_conf.h>
#include "globals.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"



void Process_IDN(uint8 *)
{
    SCPI_SEND("*IDN : Type S8-54 : Manufacturer MNIPI : Software ver. 1.0")
}



void Process_RUN(uint8 *)
{

}



void Process_STOP(uint8 *)
{

}



void Process_RESET(uint8 *)
{

}



void Process_AUTOSCALE(uint8 *)
{

}



void Process_REQUEST(uint8 *)
{
    SCPI_SEND("S8-54");
}
