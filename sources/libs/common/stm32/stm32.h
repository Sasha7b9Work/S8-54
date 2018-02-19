#pragma once

#ifdef ERROR_HANDLER
#undef ERROR_HANDLER
#endif

#define ERROR_HANDLER STM32::ErrorHandler()


class STM32
{
public:
    static void ErrorHandler();

protected:
    static void EnablePeriphery();

    static void InitHardware();
};
