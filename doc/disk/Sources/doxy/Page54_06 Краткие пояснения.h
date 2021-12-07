/*! \page page06 ������� ���������

    ��� ���������� ����������� MDK5.21.0.0.\n
    ������ ������������� �� ����� - �� ����������� ��� � ��������� � ��������.
    
    ���������� � ������������\n
    � ��������, ������ � ������� ��������� �� ������ ���������, ����������� ������� �� ������ �������������. ��������, display..\n
    �������, ������������ ������ ������ ��������/����������, ������ �������� �� �����.\n
    ������� ����������/������ �������� (���������� SSettings) ���������� � Get/Set/Inc/Dec/Add/Sub/Is. ��� �� ������ �� ��������� �������, 
    ������ ������ ��� ������/������ ��������. ����������� �������� ������� SetIncreaseSmoothlyRange(), SetDecreaseSmoothlyRange(). ��� �������� 
    FPGA_SetRange().
    
���������� ���� � ���� �������
1.  ������ CDC_Standalone �� STM32Cube V1.1.0 ������� ���������� �� 
    ������� ��� ���������� � startup_stm32f217xx.s � ����� ����������
    c startup_stm32f207xx.s, �� � ���� ������, ���� ���������� � ������������,
    ��������� ��������� ��������� ������ 10 � ������ � ����������� ����
    ����������.
    ������������ ��������, ��� � startup_stm32f217xx.s
    Stack_Size �������� �� 0x2000, � Heap_Size - �� 0x4000. ��� ���� �����
    ��������� ���������� � � startup_stm32f207xx.s\n
2.The ST's USBD_CDC_TransmitPacket() function contains a bug: it sets the 
    'transmission in progress' flag AFTER starting the actual transmission,
    so if the 'transmission done' interrupt happens too early, the flag will
    be set AFTER the interrupt and hence will never be cleared. To fix this,
    move "hcdc->TxState = 1" line before the call to USBD_LL_Transmit():
        if(hcdc->TxState == 0) {
            hcdc->TxState = 1;
            USBD_LL_Transmit(pdev, CDC_IN_EP, hcdc->TxBuffer,
            hcdc->TxLength);
            return USBD_OK;
        }
*/
