#pragma once


class LAN
{
public:

    static void Init();
    // ������� ����� ������������ �������/�������� ����� timeMS �����������. ��� ���� ��� ������� ������� �������� ����������. WARN ��������. ���� ���������.
    static void Update(uint timeMS);
};
