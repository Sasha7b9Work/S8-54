// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct ComPort
{
    static bool Open();
    static void Close();
    static bool IsOpened();
    static void Send(const char *);
    static int Receive(char *buffer, int size);
};
