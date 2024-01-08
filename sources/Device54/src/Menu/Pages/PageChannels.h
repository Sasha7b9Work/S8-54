#pragma once


class PageChannels
{
public:
    static void OnChanged_InputA(bool active);
    static void OnChanged_InputB(bool active);
    static void OnChanged_CoupleA(bool active);
    static void OnChanged_CoupleB(bool active);
    static void OnPress_BalanceA();
    static void OnPress_BalanceB();
};
