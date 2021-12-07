#pragma once


#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendBufferSynch(buffer, size)


class VCP
{
public:
    /// Эта строка передаётся с завершающими символами \\r\\n
    static void SendStringAsynch(char *format, ...);

    static void SendBufferSynch(const unsigned char *data, int size);

    static void Flush();
};
