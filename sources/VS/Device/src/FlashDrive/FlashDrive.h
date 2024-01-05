// 2024/01/05 13:25:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define FDRIVE_IS_CONNECTED FDrive::IsConnected()


namespace FDrive
{
    void Init();

    void Update();

    bool IsConnected();
}
