// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Containers/String_.h"
#include "Hardware/FDrive/FDrive.h"


bool FDrive::needOpenFileMananger = false;


void FDrive::Init()
{

}


void FDrive::GetNumDirsAndFiles(const char *, int *, int *)
{

}


String FDrive::GetNameDir(const char *, int, StructForReadDir *)
{
    return String("");
}


bool FDrive::GetNextNameDir(char *, StructForReadDir *)
{
    return false;
}


void FDrive::CloseCurrentDir(StructForReadDir *)
{

}


bool FDrive::GetNameFile(const char *, int , char *, StructForReadDir *)
{
    return false;
}


bool FDrive::GetNextNameFile(char *, StructForReadDir *)
{
    return false;
}


bool FDrive::OpenNewFileForWrite(const char *)
{
    return false;
}


bool FDrive::WriteToFile(uint8 *, int)
{
    return false;
}


bool FDrive::CloseFile()
{
    return false;
}


void FDrive::Update()
{

}


bool FDrive::IsConnected()
{
    return false;
}


void FDrive::SaveCurrentSignal()
{

}


String FDrive::CreateFileName(pchar)
{
    return String("");
}
