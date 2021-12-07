/*! \page page06 Краткие пояснения

    Для компиляции используйте MDK5.21.0.0.\n
    Пакеты устанавливать не нужно - всё необходимое идёт в комплекте с проектом.
    
    Соглашения о наименовании\n
    К функциям, доступ к которым требуется из других подсистем, добавляется префикс со знаком подчёркивания. Например, display..\n
    Функции, используемые внутри одного каталога/подсистемы, такого префикса не имеют.\n
    Функции сохранения/чтения настроек (подсистема SSettings) начинаются с Get/Set/Inc/Dec/Add/Sub/Is. Они не влияют на состояние прибора, 
    служат ТОЛЬКО для записи/чтения настроек. Исключением являются функции SetIncreaseSmoothlyRange(), SetDecreaseSmoothlyRange(). Они вызывают 
    FPGA_SetRange().
    
Замеченные баги и пути решения
1.  Пример CDC_Standalone из STM32Cube V1.1.0 успешно запускался на 
    приборе при компиляции с startup_stm32f217xx.s и также запускался
    c startup_stm32f207xx.s, но в этом случае, хотя устройство и коннектилось,
    диспетчер устройств показывал ошибку 10 и работа с устройством была
    невозможна.
    Исследование показало, что в startup_stm32f217xx.s
    Stack_Size увеличен до 0x2000, а Heap_Size - до 0x4000. При этих диких
    значениях заработало и с startup_stm32f207xx.s\n
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
