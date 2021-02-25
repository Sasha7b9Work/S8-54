#pragma once
#include "SCPI/SCPI.h"


namespace SCPI
{
    extern const StructSCPI key[]; //-V2504

    // :REG
    pCHAR FuncReg(pCHAR);
    void HintReg(String *);
}
