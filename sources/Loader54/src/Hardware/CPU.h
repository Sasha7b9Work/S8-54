#pragma once
#include "defines.h"
#include "Hardware/stm32/4XX/stm437.h"
#include "Hardware/Controls.h"

#include <usbh_core.h>




#define PRIORITY_SYS_TICK           0, 0
#define PRIORITY_FPGA_ADC           1, 0
#define PRIORITY_FPGA_P2P           1, 1
#define PRIORITY_TIMER_TIM6         2, 0
#define PRIORITY_PANEL_EXTI9_5      3, 0
#define PRIORITY_PANEL_SPI1         4, 0
#define PRIORITY_SOUND_DMA1_STREAM5 5, 0
#define PRIORITY_FLASHDRIVE_OTG     6, 0
#define PRIORITY_VCP_OTG            7, 0
#define PRIORITY_RAM_DMA2_STREAM0   8, 0
    


#define ADDR_BANK           0x60000000

#define ADDR_CDISPLAY               ((uint8 *)(ADDR_BANK + 0x080000))    // Здесь 8-битный низкоскоростной мультиплексный режим



class CPU : public STM437
{
public:
    static void Init();

    static void DeInit();

    
    class Panel
    {
    friend class CPU;
        
    private:

        static void Init();
    
        static void DeInit();
    
    public:

        static void Update();
        // Передать даннные в мк панели управления.
        static void TransmitData(uint16 data);
        // В отлюченном режиме панель лишь обновляет состояние переменной pressedButton, не выполняя больше никаких действий.
        static void Disable();

        static void Enable();
        // Ожидать нажатие клавиши.
        static Key::E WaitPressingButton();

        static bool ProcessingCommandFromPIC(uint16 command);

        static uint16 NextData();

        static Key::E PressedButton();

        static void SPI_IRQHandler();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class FSMC_
    {
    friend class CPU;
        
    public:
        static void Init();
        static void DeInit();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class FLASH_
    {
    public:
        static const uint ADDR_SECTOR_PROGRAM_0 = ((uint)0x08020000);   // Адрес первого сектора с программой

        // Очищает сектора, предназначенные для записи прошивки.
        static void Prepare();
        // Записать буфер, на который указывает указатель data, размером size, по адресу address.
        static void WriteData(uint address, uint8 *data, int size);
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class FDrive
    {
    friend class CPU;
        
    public:
        static void Init();

    public:
        static bool Update();

        static bool FileExist(char *fileName);

        static int OpenFileForRead(char *fileName);
        // Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
        static int ReadFromFile(int numBytes, uint8 *buffer);

        static void CloseOpenedFile();

        static void HCD_IRQHandler();

        class LL_
        {
        public:
            static void InitHCD(USBH_HandleTypeDef *phost);

            static void SetToggle(uint8 pipe, uint8 toggle);

            static uint8 GetToggle(uint8 pipe);
        };

        static HCD_HandleTypeDef handleHCD;

        static USBH_HandleTypeDef  handleUSBH;
    };

private:
    static void EnablePeriphery();

    static void InitHardware();

    static void InitFSMC();
};



#ifdef __cplusplus
extern "C" {
#endif

    // Прерывание на таймер
    void TIM3_IRQHandler();
    void EXTI9_5_IRQHandler();
    void SPI1_IRQHandler();
    void OTG_FS_IRQHandler();

    void NMI_Handler();
    void HardFault_Handler();
    void MemManage_Handler();
    void BusFault_Handler();
    void UsageFault_Handler();
    void SVC_Handler();
    void DebugMon_Handler();
    void PendSV_Handler();
    void SysTick_Handler();

#ifdef __cplusplus
}
#endif
