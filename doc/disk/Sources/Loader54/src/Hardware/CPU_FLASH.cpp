#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#include <stm32f4xx.h>
#include <usbh_core.h>
#pragma clang diagnostic warning "-Wpadded"
#pragma clang diagnostic warning "-Wc++98-compat-pedantic"
#include "CPU.h"



#ifdef STM32F437xx
#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */
#endif



void CPU::FLASH_::Prepare()
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef flashITD;
    flashITD.TypeErase = TYPEERASE_SECTORS;
    flashITD.Sector = FLASH_SECTOR_5;
    flashITD.NbSectors = 3;
    flashITD.VoltageRange = VOLTAGE_RANGE_3;
    uint error = 0;

    HAL_FLASHEx_Erase(&flashITD, &error);

    HAL_FLASH_Lock();
}



void CPU::FLASH_::WriteData(uint address, uint8 *data, int size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    if ((size % 4) == 0)
    {
        size /= 4;
        for (int i = 0; i < size; i++)
        {
            uint *data32 = (uint *)data;
            HAL_FLASH_Program(TYPEPROGRAM_WORD, address, (uint64_t)(data32[i]));
            address += 4;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            HAL_FLASH_Program(TYPEPROGRAM_BYTE, address++, (uint64_t)data[i]);
        }
    }

    HAL_FLASH_Lock();
}
