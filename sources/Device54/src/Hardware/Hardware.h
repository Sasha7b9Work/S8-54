#pragma once
#include "defines.h"



/** @defgroup Hardware
 *  @brief ���������� �������
 *  @{
 */

/** @defgroup Priorities
 *  @brief ���������� ���������� �������
 *  @{
 */

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

/** @}
 */

/// ������������� ���������� ��������
class Hardware
{
public:
    static void Init();
    /// ���������� ����������� ����� ����������� ��� �������� (5, 6, 7), � ������� �������� �������� ���������.
    static uint CalculateCRC32();
};

/** @}
 */
