// 2024/01/06 12:11:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/FLASH.h"


bool FLASHmem::GetData(int /*num*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
    return false;
}


void FLASHmem::DeleteAllData()
{

}


void FLASHmem::GetDataInfo(bool /*existData*/ [MAX_NUM_SAVED_WAVES])
{

}


void FLASHmem::SaveData(int /*num*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{

}


void FLASHmem::DeleteData(int /*num*/)
{

}


void FLASHmem::LoadSettings()
{

}


void FLASHmem::SaveSettings()
{

}


bool OTPmem::SaveSerialNumber(char * /*servialNumber*/)
{
    return false;
}


int OTPmem::GetSerialNumber(char /*buffer*/ [17])
{
    return 0;
}
