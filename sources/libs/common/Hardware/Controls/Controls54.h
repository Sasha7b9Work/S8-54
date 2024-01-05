#pragma once


// �������������� ������.

struct Key
{
    enum E
    {
        Empty    = 0x00,     // ������ �� ������
        ChannelA = 0x01,     // ����� 1
        Service  = 0x02,     // ������
        ChannelB = 0x03,     // ����� 2
        Display  = 0x04,     // �������
        Time     = 0x05,     // ����
        Memory   = 0x06,     // ������
        Trig     = 0x07,     // �����
        Start    = 0x08,     // ����/�����
        Cursors  = 0x09,     // �������
        Measures = 0x0a,     // �����
        Power    = 0x0b,     // �������
        Help     = 0x0c,     // ������
        Menu     = 0x0d,     // ����
        F1       = 0x0e,     // 1
        F2       = 0x0f,     // 2
        F3       = 0x10,     // 3
        F4       = 0x11,     // 4
        F5       = 0x12,     // 5
        Count            // ����� ���������� ������
    };
};

// �������������� �����������.
struct Reg
{
    enum E
    {                           //               ������� - ����������
        Empty       = 0x00,
        RangeA      = 0x14,     // �����/��� ���.A  0x9c - 0x1c
        RShiftA     = 0x15,     //                  0x9d - 0x1d
        RangeB      = 0x16,     // �����/��� ���.B  0x9e - 0x1e
        RShiftB     = 0x17,     //                  0x9f - 0x1f
        TBase       = 0x18,     // �����/���        0xa0 - 0x20
        TShift      = 0x19,     //                  0xa1 - 0x21
        TrigLev     = 0x1a,     // �������          0xa2 - 0x22
        Set         = 0x1b,     // ���������        0xa3 - 0x23
        Range1_BTN  = 0x1c,     // ������� ��������� �������� �� ����������
        RShiftA_BTN = 0x1d,     // ��������� ����
        RangeB_BTN  = 0x1e,     // ������� ��������� �������� �� ���������
        RShiftB_BTN = 0x1f,     // ��������� ����
        TBase_BTN   = 0x20,     // ��������� ������ ��������
        TShift_BTN  = 0x21,     // ��������� ����
        TrigLev_BTN = 0x22,     // ��������� ���� ������������� ��� �������������� �������������
        Set_BTN     = 0x23,
        Count
    };
};

// ����������� �������� �����������.
typedef enum
{
    Direction_Left,     // �������� ���������� �����
    Direction_Right,    // �������� ���������� ������
    Direction_None      // �������� �����������
} RegulatorDirection;

/// �������, ����������� �� ������
typedef enum
{
    C_None = 0,
    C_Reset =  0x24     ///< ������� ������. ����������� ������������� �������� ���������� ������ �� ���������� (�������+�������+������+F4)
} PanelCommand;

#define NUM_REGULATORS (R_Set - R_RangeA + 1)


const char *NameButton(Key::E);
const char *NameRegulator(Reg::E);
