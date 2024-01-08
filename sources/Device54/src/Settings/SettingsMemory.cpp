// 2024/01/05 16:51:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SettingsMemory.h"
#include "Data/Reader.h"
#include "FPGA/FPGATypes.h"
#include <stdlib.h>


int sMemory_NumPointsInChannel_()
{
    static const int numPoints[ENumPointsFPGA::Count] =
    {
        512,
        1024,
        2048,
        4096,
        8192,
        16384,
        32768
    };

    return numPoints[FPGA_ENUM_POINTS];
}


int sMemory_NumBytesInChannel_()
{
    DataSettings ds;
    ds.Fill();
    return ds.BytesInChannel();
}


ENumPointsFPGA::E NumPoints_2_ENumPoints(int numPoints)
{
    if (numPoints == 32768)      { return ENumPointsFPGA::_32k; }
    else if (numPoints == 16384) { return ENumPointsFPGA::_16k; }
    else if (numPoints == 8192)  { return ENumPointsFPGA::_8k; }
    else if (numPoints == 4096)  { return ENumPointsFPGA::_4k; }
    else if (numPoints == 2048)  { return ENumPointsFPGA::_2k; }
    else if (numPoints == 1024)  { return ENumPointsFPGA::_1k; }
    return ENumPointsFPGA::_512;
}



int ENumPoints_2_NumPoints(ENumPointsFPGA::E numPoints)
{
    static const int n[ENumPointsFPGA::Count] =
    {
        512,
        1024,
        2048,
        4096,
        8192,
        16384,
        32768
    };
    return n[(uint)numPoints];
}


void *AllocMemForChannelFromHeap(Channel ch, DataSettings *ds)
{
    int numBytes = RequestBytesForChannel(ch, ds);
    if (numBytes)
    {
        return malloc((uint)numBytes);
    }
    return 0;
}


int RequestBytesForChannel(Channel ch, DataSettings *ds)
{
    ENumPointsFPGA::E numBytes;
    PeakDetMode peakDet;

    if (ds)
    {
        numBytes = (ENumPointsFPGA::E)ENUM_BYTES(ds);
        peakDet = PEAKDET(ds);
    }
    else
    {
        numBytes = NumPoints_2_ENumPoints(NUM_BYTES(ds));
        peakDet = SET_PEAKDET;
    }

    if ((numBytes == ENumPointsFPGA::_32k) || (numBytes == ENumPointsFPGA::_16k && peakDet == PeakDet_Enabled))
    {
        if (ch == A)
        {
            return FPGA_MAX_POINTS * 2;
        }
        return 0;
    }

    return FPGA_MAX_POINTS;
}
