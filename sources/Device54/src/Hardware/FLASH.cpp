#include "globals.h"
#include "FLASH.h"
#include "Log.h"
#include "FLASH.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Hardware/Sound.h"
#include "Settings/Settings.h"
#include <limits.h>
#include <string.h>


/*
    ������� �������� ������.
    ��� �������� ������ ������������ 6 �������� �������� (ADDR_DATA_0...ADDR_DATA_5) � ���� ��������������� (ADDR_DATA_TEMP).
    ����� ��� �������� ������� ������ ������ �����������:
    0 - ADDR_DATA_0,
    1 - ADDR_DATA_0 + 32 * 1024,
    2 - ADDR_DATA_0 + 64 * 1024,
    3 - ADDR_DATA_0 + 96 * 1024,
    4 - ADDR_DATA_1,
    � �.�.
    ���������� � ��������� ��������� �������� � ArrayDatas, ������� ������������ ����� ������.
*/

// ��������� � ����������� ������
#define ADDR_SECTOR_BOOT_0      ((uint)0x08000000)          // 16k +
#define ADDR_SECTOR_BOOT_1      ((uint)0x08004000)          // 16k | ���������
#define ADDR_SECTOR_BOOT_2      ((uint)0x08008000)          // 16k /
#define ADDR_FLASH_SECTOR_3     ((uint)0x0800C000)          // 16k
#define ADDR_FLASH_SECTOR_4     ((uint)0x08010000)          // 64k
#define ADDR_SECTOR_PROGRAM_0   ((uint)0x08020000)          // 128k �������� ���������
#define ADDR_SECTOR_PROGRAM_1   ((uint)0x08040000)          // 128k �������� ���������
#define ADDR_SECTOR_PROGRAM_2   ((uint)0x08060000)          // 128k �������� ���������
#define ADDR_FLASH_SECTOR_8     ((uint)0x08080000)          // 128k
#define ADDR_FLASH_SECTOR_9     ((uint)0x080A0000)          // 128k
#define ADDR_SECTOR_RESOURCES   ((uint)0x080C0000)          // 128k ����������� � �������� �������
#define ADDR_SECTOR_SETTINGS    ((uint)0x080E0000)          // 128k ���������
#define ADDR_FLASH_SECTOR_12    ((uint)0x08100000)          // 16k
#define ADDR_FLASH_SECTOR_13    ((uint)0x08104000)          // 16k
#define ADDR_FLASH_SECTOR_14    ((uint)0x08108000)          // 16k
#define ADDR_FLASH_SECTOR_15    ((uint)0x0810C000)          // 16k
#define ADDR_DATA_DATA          ((uint)0x08110000)          // 64k  ����� ����� ��������� ������� ������� � ������ �������
#define ADDR_DATA_0             ((uint)0x08120000)          // 128k +
#define ADDR_DATA_1             ((uint)0x08140000)          // 128k |
#define ADDR_DATA_2             ((uint)0x08160000)          // 128k |
#define ADDR_DATA_3             ((uint)0x08180000)          // 128k | ����� ����� ��������� ���������� ������
#define ADDR_DATA_4             ((uint)0x081A0000)          // 128k |
#define ADDR_DATA_5             ((uint)0x081C0000)          // 128k |
#define ADDR_DATA_TEMP          ((uint)0x081E0000)          // 128k /

#define SIZE_SECTOR_SETTINGS    (128 * 1024)                // ������ �������, ���� ����������� ���������, � ������

#define SIZE_SECTOR_128         (128 * 1024)

typedef struct
{
    struct ElementData
    {
        uint address;               // �����, �� �������� �������� ������ ������. ��� ������� ������ ���� ����� ��������, ��, ��� �� �����,
                                    // ������������ � ��������� - �������� MAX_UINT ��������, ��� ������ ����.
                                    // �������� datas[0].address == 0 ��������, ��� ������� ���� � ����� ���������� � ����������
        DataSettings ds;
    } datas[MAX_NUM_SAVED_WAVES];
} ArrayDatas;



#undef CLEAR_FLASH_FLAGS
#ifdef STM32F437xx
#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */
#endif


#define READ_BYTE(address) (*((volatile uint8 *)address))

#define READ_HALF_WORD(address) (*((volatile uint16 *)address))

#define READ_WORD(address) (*((volatile uint *)address))



// ��� ��������

// ��� ������
static ArrayDatas* CurrentArray();              // ���������� ����� ����������� ������� � �������� ������
static uint AddressSectorForData(int num);          // ���������� ����� �������, � ������� ��������� ������ � ������� num
static uint AddressForData(int num);                // ���������� �����, �� �������� ������ ���� ��������� ������ num
static void SaveArrayDatas(ArrayDatas array);       // ������������ ������� ArrayDatas

static uint AddressSectorForAddress(uint address);  // ���������� ����� �������, �������� ������������ ����� address

static void WriteWord(uint address, uint word);
static void WriteBufferWords(uint address, void *buffer, int numWords);
static void WriteBufferBytes(uint address, void *buffer, int numBytes);
static void ReadBufferBytes(uint addressSrc, void *bufferDest, int size);
static bool EraseSector(uint startAddress);
static uint GetSector(uint startAddress);
 

void FLASHmem::LoadSettings()
{
    if (READ_HALF_WORD(ADDR_SECTOR_SETTINGS) != 0xffff) // ���������� ����� ������� ��������, ��� ��������� ��� �����������
    {
        // ������� ������� ������� � ������������ �����������. ��������� ������������ ��������������� ���� �� ������ �� �������, ������� 1024
        int address = ADDR_SECTOR_SETTINGS;

        do
        {
            address += 1024;
        } while (READ_HALF_WORD(address) != 0xffff && address < (int)(ADDR_SECTOR_SETTINGS + SIZE_SECTOR_SETTINGS));

        address -= 1024;

        // ������ � Settings set ���������� ����, ��������� � (int16)*address
        ReadBufferBytes((uint)address, &set, READ_HALF_WORD(address));
    }
}



void FLASHmem::SaveSettings()
{
    Sound::WaitForCompletion();

    // ���������� � Settings.size ������� ������ ��������� Settings
    set.size = sizeof(Settings);

    // ������� ������ ��������� ����� ������.
    int address = ADDR_SECTOR_SETTINGS;
    while (READ_HALF_WORD(address) != 0xffff &&                     // ���� �� ������, �������� 1024, �� �������� ���������
           (uint)address < (ADDR_SECTOR_SETTINGS + SIZE_SECTOR_SETTINGS)) // � �� �� ����� �� ������� ������� ��������
    {
        address += 1024;                                            // ��������� �� ��������� �������������� ������� ������
    }

    if (address == ADDR_SECTOR_SETTINGS + SIZE_SECTOR_SETTINGS)     // ���� ����� ��������� �� ��������� ������� ������� ��� ������ ��������
    {
        EraseSector(ADDR_SECTOR_SETTINGS);                          // �� ������� � ������ ����������� ����� ��������
        address = ADDR_SECTOR_SETTINGS;
    }

    WriteBufferBytes((uint)address, &set, sizeof(Settings));
}



static void WriteWord(uint address, uint word)
{
    CLEAR_FLASH_FLAGS

    HAL_FLASH_Unlock();
    if(HAL_FLASH_Program(TYPEPROGRAM_WORD, address, (uint64)word) != HAL_OK)
    {
        LOG_ERROR_TRACE("�� ���� �������� � ������");
    }
    HAL_FLASH_Lock();
}



static void WriteBufferWords(uint address, void *buffer, int numWords)
{
    HAL_FLASH_Unlock();
    for(int i = 0; i < numWords; i++)
    {
        if(HAL_FLASH_Program(TYPEPROGRAM_WORD, address, (uint64)(((uint *)buffer)[i])) != HAL_OK)
        {
            LOG_ERROR_TRACE("�� ���� �������� � ����-������");
        }
        address += sizeof(uint);
    }
    HAL_FLASH_Lock();
}



static void ReadBufferBytes(uint addressSrc, void *bufferDest, int size)
{
    uint8 *src = (uint8 *)addressSrc;
    uint8 *dest = (uint8 *)bufferDest;

    for (int i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
}



static bool EraseSector(uint startAddress)
{
    if (GetSector(startAddress) == UINT_MAX)    // ���� ����������� ������
    {
        return false;
    }

    CLEAR_FLASH_FLAGS

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef flashITD;
    flashITD.TypeErase = TYPEERASE_SECTORS;
    flashITD.Sector = GetSector(startAddress);
    flashITD.NbSectors = 1;
    flashITD.VoltageRange = VOLTAGE_RANGE_3;

    uint error = 0;

    Sound::WaitForCompletion();

    HAL_FLASHEx_Erase(&flashITD, &error);

    HAL_FLASH_Lock();

    return true;
}


#ifdef STM32F437xx

static uint GetSector(uint startAddress)
{
    typedef struct
    {
        uint number;
        uint startAddress;
    } StructSector;

    static const StructSector sectors[] =
    {
        {FLASH_SECTOR_11, ADDR_SECTOR_SETTINGS},    // ���� ����������� ����������� ���������       128 ��
        {FLASH_SECTOR_16, ADDR_DATA_DATA},          // ������ ������� ������                        64 ��
        {FLASH_SECTOR_17, ADDR_DATA_0},             // + 
        {FLASH_SECTOR_18, ADDR_DATA_1},             // |
        {FLASH_SECTOR_19, ADDR_DATA_2},             // |
        {FLASH_SECTOR_20, ADDR_DATA_3},             // | ����� ���������� ������                    6 � 128�� = 768 ��
        {FLASH_SECTOR_21, ADDR_DATA_4},             // |
        {FLASH_SECTOR_22, ADDR_DATA_5},             // |
        {FLASH_SECTOR_23, ADDR_DATA_TEMP},          // /
        {0, 0}
    };

    int i = 0;
    while (sectors[i].startAddress)
    {
        if (sectors[i].startAddress == startAddress)
        {
            return sectors[i].number;
        }
        i++;
    }

    LOG_ERROR_TRACE("������������ ����� ������� %d", startAddress);
    return UINT_MAX;
}
#endif



void FLASHmem::GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        existData[i] = ExistData(i);
    }
}



bool FLASHmem::ExistData(int num)
{
    return (CurrentArray()->datas[num].address != UINT_MAX);
}



void FLASHmem::DeleteAllData()
{
    EraseSector(ADDR_DATA_DATA);
    EraseSector(ADDR_DATA_0);
    EraseSector(ADDR_DATA_1);
    EraseSector(ADDR_DATA_2);
    EraseSector(ADDR_DATA_3);
    EraseSector(ADDR_DATA_4);
    EraseSector(ADDR_DATA_5);
    EraseSector(ADDR_DATA_TEMP);
}



void FLASHmem::DeleteData(int num)
{
    Sound::WaitForCompletion();

    if (!ExistData(num))
    {
        return;
    }

    /*
        ����� ������� ������ ������, ��������� ������ � ������� �� ��������������� ������, ����� ��� ������, � ����� ��������� ��� ����� �������,
        ����� ���, ������� ������ ������� ������.
        ����� ����������� ����� ArrayDatas, � ������� ��������� address � MAX_UINT.
    */

    EraseSector(ADDR_DATA_TEMP);

    uint addressSector = AddressSectorForData(num);

    WriteBufferWords(ADDR_DATA_TEMP, (void *)addressSector, SIZE_SECTOR_128 / 4);

    EraseSector(addressSector);

    int numNotWritted = num % 4;                // ����� �� ������� ��������, ������� �� ����� ������������, ������ ��� � ��� �������� ���� ������, 
                                                // ������� ����� �������. ��� ����� ������� �� ������� �� 4, ������ ��� � ������ ������� �������� 
                                                // ����� 4 ������ ������
    uint sizeQuartPart = SIZE_SECTOR_128 / 4;   // ������ ����� �������� ����� ������� � ������

    for (uint i = 0; i < 4; i++)
    {
        if (i != (uint)numNotWritted)
        {
            WriteBufferWords(addressSector + sizeQuartPart * i, (void *)(ADDR_DATA_TEMP + sizeQuartPart * i), (int)sizeQuartPart / 4);
        }
    }
    
    ArrayDatas array = *CurrentArray();

    array.datas[num].address = UINT_MAX;                        // ����� ������� ����, ��� ������ num ��� �� ��������
    memset(&array.datas[num].ds, 0xff, sizeof(DataSettings));   // ��������� ����� ��� ��������� DataSettings ��������� 0xff, ����� ��������� ���������
                                                                // ������� ������ � �������
    SaveArrayDatas(array);
}



static uint AddressSectorForData(int num)
{
    return AddressSectorForAddress(AddressForData(num));
}



static uint AddressForData(int num)
{
    return (uint)(ADDR_DATA_0 + (SIZE_SECTOR_128) / 4 * (uint)num);
}



static uint AddressSectorForAddress(uint address)
{
    const uint addresses[] =
    {
        ADDR_SECTOR_BOOT_0,
        ADDR_SECTOR_BOOT_1,
        ADDR_SECTOR_BOOT_2,
        ADDR_FLASH_SECTOR_3,
        ADDR_FLASH_SECTOR_4,
        ADDR_SECTOR_PROGRAM_0,
        ADDR_SECTOR_PROGRAM_1,
        ADDR_SECTOR_PROGRAM_2,
        ADDR_FLASH_SECTOR_8,
        ADDR_FLASH_SECTOR_9,
        ADDR_SECTOR_RESOURCES,
        ADDR_SECTOR_SETTINGS,
        ADDR_FLASH_SECTOR_12,
        ADDR_FLASH_SECTOR_13,
        ADDR_FLASH_SECTOR_14,
        ADDR_FLASH_SECTOR_15,
        ADDR_DATA_DATA,
        ADDR_DATA_0,
        ADDR_DATA_1,
        ADDR_DATA_2,
        ADDR_DATA_3,
        ADDR_DATA_4,
        ADDR_DATA_5,
        ADDR_DATA_TEMP,
        0
    };

    if (address < addresses[0] || address >= ADDR_DATA_TEMP + SIZE_SECTOR_128)
    {
        LOG_ERROR_TRACE("����� �� ��������� ����");
    }

    const uint *pointer = &addresses[1];

    while (*pointer)
    {
        if (address < *pointer)
        {
            pointer--;
            return *pointer;
        }
        pointer++;
    }

    return ADDR_DATA_TEMP;
}



static void WriteBufferBytes(uint address, void *buffer, int size)
{
    Sound::WaitForCompletion();
    
    CLEAR_FLASH_FLAGS

    HAL_FLASH_Unlock();
    for(int i = 0; i < size; i++)
    {
        uint64 data = ((uint8 *)buffer)[i];
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data);
        address++;
    }
    HAL_FLASH_Lock();
}



void FLASHmem::SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB)
{
    Sound::WaitForCompletion();

    DeleteData(num);              // ������� ����� ������ �� ����� ������

    // ������ �������� ������ ����� ������

    uint address = AddressForData(num);
    int sizeChannel = NUM_BYTES(ds);

    if (ENABLED_A(ds))
    {
        WriteBufferBytes(address, dataA, sizeChannel);
        address += (uint)sizeChannel;
    }

    if (ENABLED_B(ds))
    {
        WriteBufferBytes(address, dataB, sizeChannel);
    }

    ArrayDatas *array = CurrentArray();

    uint addrADDR = (uint)&array->datas[num].address;
    WriteWord(addrADDR, AddressForData(num));

    uint addrDS = (uint)&array->datas[num].ds;
    WriteBufferBytes(addrDS, (void *)ds, sizeof(DataSettings));

    DataSettings dataSettings = {};

    memcpy(&dataSettings, ds, sizeof(DataSettings));
}



static ArrayDatas* CurrentArray()
{
    ArrayDatas *array = (ArrayDatas*)ADDR_DATA_DATA;

    while (array->datas[0].address == 0)    // ������� �������� ��������, ��� ������� �����.
    {
        array++;                            // ������� ��������� � ����������, ����� ��������� ��� ��� ������������
    }

    return array;
}



static void SaveArrayDatas(ArrayDatas array)
{
    uint address = (uint)CurrentArray() + sizeof(ArrayDatas);   // �����, �� �������� ����� ���������� array

    WriteWord((uint)&CurrentArray()->datas[0].address, 0);      // ������� � ������� ������ ������� ����, ��� �� ��� �� �������

    if (address + sizeof(ArrayDatas) >= ADDR_DATA_0)            // ���� ��� ������ array � ������� ��� �����
    {
        EraseSector(ADDR_DATA_DATA);                            // �� ����� ���� ������
        address = ADDR_DATA_DATA;                               // � ���������� array ����� � ������ ���
    }

    WriteBufferBytes(address, (void *)&array, sizeof(ArrayDatas));
}



bool FLASHmem::GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB)
{
    ArrayDatas *pArray = CurrentArray();
    ArrayDatas array = *pArray;

    uint addrData = array.datas[num].address;

    if (addrData == UINT_MAX)   // ���� ������ ���
    {
        return false;           // �������� �� ���� ������������
    }

    *ds = array.datas[num].ds;

    if (ENABLED_A(ds))
    {
        memcpy(dataA, (void *)addrData, (uint)NUM_BYTES(ds));
        addrData += (uint)NUM_BYTES(ds);
    }

    if (ENABLED_B(ds))
    {
        memcpy(dataB, (void *)addrData, (uint)NUM_BYTES(ds));
    }

    return true;
}


bool OTPmem::SaveSerialNumber(char *servialNumber)
{
    // ������� ������ ������ ������ ������ 16 ���� � ������� OTP, ������� � ������
    uint8 *address = (uint8 *)FLASH_OTP_BASE;

    while ((*address) != 0xff &&                    // *address != 0xff ��������, ��� ������ � ��� ������ ��� �������������
           address < (uint8 *)FLASH_OTP_END - 16)
    {
        address += 16;
    }

    if (address < (uint8 *)FLASH_OTP_END - 16)
    {
        WriteBufferBytes((uint)address, (uint8 *)servialNumber, (int)strlen(servialNumber) + 1);
        return true;
    }

    return false;
}


int OTPmem::GetSerialNumber(char buffer[17])
{
    // \todo �������� - ������ �������������� �������. ������ ������ ������ �������� ������� �����, ������� � ��� ��������, � �� 16, ��� ������.

    const int allShotsMAX = 512 / 16;   // ������������ ����� ������� � OTP ��������� ������.

    uint8 *address = (uint8 *)FLASH_OTP_END - 15;

    do
    {
        address -= 16;
    } while(*address == 0xff && address > (uint8 *)FLASH_OTP_BASE);

    if (*address == 0xff)   // �� ����� ������ � �����������, ����� �� ���������� ������ OTP
    {
        buffer[0] = 0;
        return allShotsMAX;
    }

    strcpy(buffer, (char *)address);

    return allShotsMAX - (address - (uint8 *)FLASH_OTP_BASE) / 16 - 1;
}

#undef CLEAR_FLASH_FLAGS
#undef READ_WORD
#undef READ_HALF_WORD
