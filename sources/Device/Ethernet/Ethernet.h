#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Ethernet
 *  @brief ������ � Ethernet
 *  @{
 */

class Ethernet
{
public:
    /// �������������
    static void Init();
    /// ������� ����� ������������ �������/�������� ����� timeMS �����������. ��� ���� ��� ������� ������� �������� ����������.
    /// \todo ��������. ���� ���������.
    static void Update(uint timeMS);
};

/** @}
 */
