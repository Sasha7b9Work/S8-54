#pragma once

/// ����� ID ���� �������������� � ���� �������
typedef enum
{
    Page_Main,                      ///< ������� �������� ����
    Page_Display,                   ///< ������
    Page_Display_Accum,             ///< ������� - ����������
    Page_Display_Average,           ///< ������� - ����������
    Page_Display_Grid,              ///< ������� - �����
    Page_Display_Settings,          ///< ������� - ���������
    Page_Display_Settings_Colors,   ///< ������� - ��������� - �����
    Page_ChannelA,                  ///< ����� 1
    Page_ChannelB,                  ///< ����� 2
    Page_Trig,                      ///< �����
    Page_Trig_Search,               ///< ����� - �����
    Page_Time,                      ///< ���������
    Page_Cursors,                   ///< �������
    PageSB_Cursors_Set,             ///< ������� - ����������
    Page_Memory,                    ///< ������
    PageSB_Memory_Last,             ///< ������ - ���������
    PageSB_Memory_Internal,         ///< ������ - ����� ��
    Page_Memory_Drive,              ///< ������ - ����� ��
    PageSB_Memory_Drive_Manager,    ///< ������ - ����� �� - �������
    PageSB_Memory_Drive_Mask,       ///< ������ - ����� �� - �����
    PageSB_Memory_SetName,          ///< \brief ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ��������������� ����� 
                                    ///< ����� �� - ��� �����
    Page_Measures,                  ///< ���������
    PageSB_Measures_Tune,           ///< ��������� - ���������
    Page_Service,                   ///< ������
    Page_Service_Calibrator,        ///< ������ - ����������
    Page_Service_Ethernet,          ///< ������ - ETHERNET
    Page_Service_FreqMeter,         ///< ������ - ����������
    Page_Service_Sound,             ///< ������ - ����
    Page_Service_RTC,               ///< ������ - �����
    Page_Service_FFT,               ///< ������ - ������
    PageSB_Service_FFT_Cursors,     ///< ������ - ������ - �������
    PageSB_Service_Function,        ///< ������ - �������
    PageSB_Service_Information,     ///< ������ - ����������
    PageSB_Service_Recorder,        ///< ������ - �����������
    PageSB_Help,                    ///< ������
    Page_Debug,                     ///< �������
    Page_Debug_Console,             ///< ������� - �������
    Page_Debug_Console_Registers,   ///< ������� - ������� - ��������
    Page_Debug_ADC,                 ///< ������� - ���
    Page_Debug_ADC_Balance,         ///< ������� - ��� - ������
    Page_Debug_ADC_Stretch,         ///< ������� - ��� - ��������
    Page_Debug_ADC_Shift,           ///< ������� - ��� - ��� ����
    Page_Debug_ADC_AltShift,        ///< ������� - ��� - ��� ���� ���
    Page_Debug_Rand,                ///< ������� - ����-���
    Page_Debug_Channels,            ///< ������� - ������
    PageSB_Debug_Settings,          ///< ������� - ���������
    PageSB_Debug_SerialNumber,      ///< ������� - �/�
    Page_NumPages,
    Page_NoPage
} NamePage;
