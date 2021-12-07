#pragma once


namespace LAN
{
    void Init();

    // Функция будет обрабатывать входной/выходной буфер timeMS миллисекунд. Это надо для передчи больших массивов информации. WARN говнокод. Надо исправить.
    void Update(uint timeMS);

    extern bool cableIsConnected;
};
