#pragma once




#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendDataSynch(buffer, size)
#define VCP_FLUSH()


class VCP
{
public:
    /// ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringAsynch(char *format, ...);

    static void SendDataSynch(const unsigned char *data, int size);
};
