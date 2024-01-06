// 2024/01/05 13:31:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FlashDrive/FlashDrive.h"


void FDrive::Init()
{

}


void FDrive::Update()
{

}


bool FDrive::IsConnected()
{
    return false;
}


void FDrive::Mount()
{

}


bool FDrive::CloseFile(StructForWrite * /*structForWrite*/)
{
    return true;
}


bool FDrive::WriteToFile(uint8 * /*data*/, int /*sizeData*/, StructForWrite * /*structForWrite*/)
{
    return true;
}


bool FDrive::OpenNewFileForWrite(const char * /*fullPathToFile*/, StructForWrite * /*structForWrite*/)
{
    return true;
}
