#pragma once


namespace LAN
{
    void Init();

    // ������� ����� ������������ �������/�������� ����� timeMS �����������. ��� ���� ��� ������� ������� �������� ����������. WARN ��������. ���� ���������.
    void Update(uint timeMS);

    extern bool cableIsConnected;
};
