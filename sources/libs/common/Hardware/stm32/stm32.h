#pragma once

#ifdef ERROR_HANDLER
#undef ERROR_HANDLER
#endif

#define ERROR_HANDLER STM32::ErrorHandler()


class STM32
{
public:
    static void ErrorHandler();

protected:
};

#ifdef __cplusplus
extern "C" {
#endif

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
