#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include <iostream>
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/textfile.h>
#pragma warning(pop)
#include <windows.h>


#define SIZE_BUFFER 100
#define FILE_NAME   wxT("log.log")

static bool consoleIsExist = false;


static wxTextFile *file = nullptr;


static void AddToSystemConsole(const char *message)
{
    if(Console::IsShown())
    {
        if(!consoleIsExist)
        {
            consoleIsExist = true;

            AllocConsole();

            wxRemoveFile(FILE_NAME);

            file = new wxTextFile(FILE_NAME);

            file->Create();

            file->Close();
        }

        std::cout << message << std::endl;

        file->Open();

        file->AddLine(message);

        file->Write();

        file->Close();
    }
}


void Log::Message(const char *message)
{
    Console::AddString(const_cast<char *>(message));

    AddToSystemConsole(message);
}


void Log::Message(const char *f, int line, const char *message)
{
    String msg("%s %d %s", f, line, message);

    Console::AddString(msg.c_str());

    AddToSystemConsole(msg.c_str());
}
