#include "defines.h"
#include "SCPI/SCPI.h"
#include "commonSCPI.h"
#include <usbd_conf.h>
#include "globals.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "FPGA/FPGA.h"


void COMMON::IDN(uint8 *)
{
    SCPI_SEND("*IDN : Type S8-54 : Manufacturer MNIPI : Software ver. %s", VERSION_SOFTWARE)
}



void COMMON::RUN(uint8 *)
{
    SCPI::INPUT::needRunFPGA = true;
}



void COMMON::STOP(uint8 *)
{
    SCPI::INPUT::needStopFPGA = true;
}



void COMMON::RESET(uint8 *)
{
    SCPI::INPUT::needReset = true;
}



void COMMON::AUTOSCALE(uint8 *)
{
    SCPI::INPUT::needAutoscale = true;
}



void COMMON::REQUEST(uint8 *)
{
    SCPI_SEND("S8-54");
}
