// 2024/01/05 13:25:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct ComPort
{
    static bool Open();
    static void Close();
    static bool IsOpened();
    static void Send(const char *);
    static int Receive(char *buffer, int size);
};
