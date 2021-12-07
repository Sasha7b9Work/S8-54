#include "defines.h"
#include "SCPI.h"
#include "commonSCPI.h"
#include <usbd_conf.h>
#include "globals.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "FPGA/FPGA.h"


void Process_IDN(uint8 *)
{
    SCPI_SEND("*IDN : Type S8-54 : Manufacturer MNIPI : Software ver. %s", VERSION_SOFTWARE)
}



void Process_RUN(uint8 *)
{
    SCPI::INPUT::needRunFPGA = true;
}



void Process_STOP(uint8 *)
{
    SCPI::INPUT::needStopFPGA = true;
}



void Process_RESET(uint8 *)
{
    SCPI::INPUT::needReset = true;
}



void Process_AUTOSCALE(uint8 *)
{
    SCPI::INPUT::needAutoscale = true;
}



void Process_REQUEST(uint8 *)
{
    SCPI_SEND("S8-54");
}
