#pragma once

#define ERROR_HANDLER() STM32::ErrorHandler()


class STM32
{
public:
    static void ErrorHandler();

protected:
};
