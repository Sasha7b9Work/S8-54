#include "DataStorage.h"
#include "Log.h"
#include "FPGA/FPGA.h"
#include "Hardware/RAM.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Возвращает 0, если канал выключен
static uint8 *AddressChannel(DataSettings *ds, Channel ch);
static bool DataSettingsIsEquals(const DataSettings *ds1, const DataSettings *ds2);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int SIZE_POOL = 0;

static uint *sumA_RAM = 0;        // Сумма первого канала
static uint *sumB_RAM = 0;

static uint16 *limitUpA_RAM = 0;
static uint16 *limitUpB_RAM = 0;
static uint16 *limitDownA_RAM = 0;
static uint16 *limitDownB_RAM = 0;

static int iFirst = 0;          // Номер в массиве datas первого сохранённого сигнала
static int iLast = 0;           // Номер в массиве datas последнего сохранённого сигнала

static float *aveDataA_RAM = 0; // В этих массивах хранятся усреднённые значения, подсчитанные по приблизительному алгоритму.
static float *aveDataB_RAM = 0;

static bool newSumCalculated[NumChannels] = {true, true};   // Если true, то новые суммы рассчитаны, и нужно повторить расчёт среднего
static int numElementsInStorage = 0;


// Для поточечного режима фрейм бегущего кадра
static uint8 *frameP2P = 0;
static int numPointsP2P = 0;            // Когда в последнем фрейме хранятся точки для поточечного вывода, то здесь хранится количество точек
static DataSettings dsP2P = {0};        // Настройки поточечного режима    
static bool inFrameP2Pmode = false;     // Если true - сейчас поточечный режим

#define NUM_DATAS 999
static DataSettings datas[NUM_DATAS];
static uint8 gDataAve[NumChannels][FPGA_MAX_POINTS];

#define ADDRESS_DATA(ds)        ((ds)->addr)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClearLimitsAndSums()
{
    int numBytesA = RequestBytesForChannel(A, 0);
    int numBytesB = RequestBytesForChannel(B, 0);

    RAM::MemClear(limitUpA_RAM, numBytesA);
    RAM::MemClear(limitUpB_RAM, numBytesB);
    RAM::MemClear(limitDownA_RAM, numBytesA);
    RAM::MemClear(limitDownB_RAM, numBytesB);
    RAM::MemClear(sumA_RAM, numBytesA * 4);
    RAM::MemClear(sumB_RAM, numBytesB * 4);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::Clear()
{
    SIZE_POOL = RAM8(DS_POOL_END) - RAM8(DS_POOL_BEGIN);

    sumA_RAM = (uint *)RAM8(DS_SUM_A);
    sumB_RAM = (uint *)RAM8(DS_SUM_B);

    limitUpA_RAM = RAM16(DS_LIMIT_UP_A);
    limitUpB_RAM = RAM16(DS_LIMIT_UP_B);
    limitDownA_RAM = RAM16(DS_LIMIT_DOWN_A);
    limitDownB_RAM = RAM16(DS_LIMIT_DOWN_B);
    frameP2P = RAM8(DS_P2P_FRAME);

    aveDataA_RAM = (float *)RAM8(DS_AVE_DATA_A);
    aveDataB_RAM = (float *)RAM8(DS_AVE_DATA_B);

    iFirst = 0;
    iLast = 0;
    ADDRESS_DATA(&datas[iFirst]) = ADDRESS_DATA(&datas[iLast]) = 0;

    for(int i = 0; i < NUM_DATAS; i++)
    {
        ADDRESS_DATA(&datas[i]) = 0;  // Пишем признак того, что ячейка свободна
    }

    numElementsInStorage = 0;

    ClearLimitsAndSums();
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::CalculateAroundAverage(uint8 *dataA, uint8 *dataB, DataSettings *dss)
{
    if (!dataA && !dataB)
    {
        return;
    }

    int numAveData = NumElementsWithCurrentSettings();

    int size = NUM_BYTES(dss);

    if (numAveData <= 1)
    {
        for (int i = 0; i < size; i++)
        {
            aveDataA_RAM[i] = dataA[i];
            aveDataB_RAM[i] = dataB[i];
        }
    }
    else
    {
        numAveData = sDisplay_NumAverage();

        if(numAveData > NumElementsInStorage())
        {
            numAveData = NumElementsInStorage();
        }

        float numAveDataF = (float)numAveData;
        float numAveDataFless = numAveDataF - 1.0f;
        float numAveDataInv = 1.0f / numAveDataF;
        float *aDataA = &aveDataA_RAM[0];
        float *aDataB = &aveDataB_RAM[0];
        uint8 *dA = &dataA[0];
        uint8 *dB = &dataB[0];
        float *endData = &aveDataA_RAM[size];

        do 
        {
            *aDataA = ((*aDataA) * numAveDataFless + (float)(*dA++)) * numAveDataInv;
            ++aDataA;
            *aDataB = ((*aDataB) * numAveDataFless + (float)(*dB++)) * numAveDataInv;
            ++aDataB;
        } while (aDataA < endData);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
// Возвращает количество байт, требуемых для записи данных с настройками ds
static int SizeData(DataSettings *ds)
{
    /// \todo должна быть глобальная функция для расчёта размера данных. Какжется, где-то она есть в функциях DataSettings.

    int size = 0;

    int numBytesInChannel = NUM_BYTES(ds);

    if(ENABLED_A(ds))
    {
        size += numBytesInChannel;
        if(PEAKDET(ds) != PeakDet_Disable)
        {
            size += numBytesInChannel;
        }
    }

    if(ENABLED_B(ds))
    {
        size += numBytesInChannel;
        if(PEAKDET(ds) != PeakDet_Disable)
        {
            size += numBytesInChannel;
        }
    }
    return size;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DeleteFirst()
{
    ADDRESS_DATA(&datas[iFirst]) = 0;
    iFirst++;
    if(iFirst == NUM_DATAS)
    {
        iFirst = 0;
    }
    numElementsInStorage--;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
// Находит место для записи данных во внешнее ОЗУ.
// По выходу из функции элемент datas[iLast] содержит ds, a ds содержит адрес для записи во внешнее ОЗУ
// При этом функция сама модернизируе iFirst, iLast, addrData элементов datas (0 указывает на то, что элемент свободен)
static void PrepareLastElemForWrite(DataSettings *ds)
{
    // Если хранилище пустое
    if(ADDRESS_DATA(&datas[iFirst]) == 0)
    {
        iFirst = iLast = 0;
        ADDRESS_DATA(ds) = RAM8(DS_POOL_BEGIN);
        datas[iFirst] = *ds;
        return;
    }

    // Если в хранилище один элемент
    if(iFirst == iLast)
    {
        iLast = iFirst + 1;
        ADDRESS_DATA(ds) = ADDRESS_DATA(&datas[iFirst]) + SizeData(&datas[iFirst]);
        datas[iLast] = *ds;
        return;
    }

    // Если в хранилище максимально возможное число эелементов
    if(numElementsInStorage >= NUM_DATAS)
    {
        DeleteFirst();
    }

    uint8 *addrWrite = 0;

    int size = SizeData(ds);

    // Сначала найдём свободное место в ОЗУ

    volatile bool run = true;
    while(run)
    {
        uint8 *addrFirst = ADDRESS_DATA(&datas[iFirst]);
        uint8 *addrLast = ADDRESS_DATA(&datas[iLast]);

        if(addrLast > addrFirst)                                                   // Данные в памяти сохранены в порядке возрастания
        {
            int memFree = RAM8(DS_POOL_END) - addrLast - SizeData(&datas[iLast]);     // Столько памяти осталось за последним элементом

            if(memFree >= size)                                                    // Памяти за последним элементом достаточно
            {
                addrWrite = addrLast + SizeData(&datas[iLast]);
                break;
            }
            else                                                                    // Памяти за последним элементом не хватает.
            {
                if(addrFirst - RAM8(DS_POOL_BEGIN) < size)                       // Если в начале меньше памяти, чем необходимо
                {
                    DeleteFirst();                                              // Удаляем один элемент с начала
                }
                else
                {
                    addrWrite = RAM8(DS_POOL_BEGIN);
                    break;
                }
            }
        }
        else
        {
            int memFree = addrFirst - addrLast - SizeData(&datas[iFirst]);

            if(memFree >= size)
            {
                addrWrite = addrLast + SizeData(&datas[iLast]);
                break;
            }
            else
            {
                if(addrFirst - addrLast - SizeData(&datas[iLast]) < size)
                {
                    DeleteFirst();
                }
                else
                {
                    addrWrite = ADDRESS_DATA(&datas[iLast]) + SizeData(&datas[iLast]);
                    break;
                }
            }
        }
    }

    // Теперь найдём последний элемент
    iLast++;
    if(iLast == NUM_DATAS)
    {
        iLast = 0;
    }
    ADDRESS_DATA(ds) = addrWrite;
    datas[iLast] = *ds;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PushData(DataSettings *ds, uint8 *dataA, uint8 *dataB)
{
    PrepareLastElemForWrite(ds);

    int numBytes = NUM_BYTES(ds);

    if(dataA)
    {
        if (ENABLED_A(ds))
        {
            RAM::MemCpy16(dataA, AddressChannel(ds, A), numBytes);
        }
    }
    else
    {
        RAM::MemSet_Sinch(AddressChannel(ds, A), NONE_VALUE, numBytes);  // Для режима поточечного вывода - заполняем одним значением
    }

    if(dataB)
    {
        if (ENABLED_B(ds))
        {
            RAM::MemCpy16(dataB, AddressChannel(ds, B), numBytes);
        }
    }
    else
    {
        RAM::MemSet_Sinch(AddressChannel(ds, B), NONE_VALUE, numBytes);  // Для режима поточечного вывода - заполянем одним значением
    }

    numElementsInStorage++;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void BeginLimits(uint8 *dataA, uint8 *dataB, int numElements)
{
    for(int i = 0; i < numElements / 2; i++)
    {
        limitUpA_RAM[i] = limitDownA_RAM[i] = ((uint16 *)dataA)[i];
        limitUpB_RAM[i] = limitDownB_RAM[i] = ((uint16 *)dataB)[i];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DataSettings* Storage::DataSettingsFromEnd(int indexFromEnd)
{
    int index = 0;

    if(indexFromEnd <= iLast)
    {
        index = iLast - indexFromEnd;
    }
    else
    {
        indexFromEnd -= iLast;
        index = NUM_DATAS - indexFromEnd;
    }

    return &datas[index];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DataSettings* Storage::GetSettingsDataFromEnd(int fromEnd)
{
    return DataSettingsFromEnd(fromEnd);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::CalculateLimits(uint8 *dataA, uint8 *dataB, DataSettings *dss)
{
#define SET_LIMIT(d, up, down)          \
    data = RAM::ReadByte(d + i);        \
    limitUp = RAM::ReadByte(up + i);    \
    limitDown = RAM::ReadByte(down + i);\
    if(data > limitUp)                  \
    {                                   \
        RAM::WriteByte(up + i, data);   \
    }                                   \
    if(data < limitDown)                \
    {                                   \
        RAM::WriteByte(down + i, data); \
    }

    int numBytes = NUM_BYTES(dss);

    if(NumElementsInStorage() == 0 || NUM_MIN_MAX == 1 || (!DataSettingsIsEquals(dss, GetSettingsDataFromEnd(0))))
    {
        BeginLimits(dataA, dataB, numBytes);
    }
    else
    {
        int allDatas = NumElementsWithSameSettings();
        LIMITATION(allDatas, 1, NUM_MIN_MAX);

        if(NumElementsWithSameSettings() >= NUM_MIN_MAX)
        {
            BeginLimits(dataA, dataB, numBytes);
            allDatas--;
        }

        for(int numData = 0; numData < allDatas; numData++)
        {
            dataA = GetData_RAM(A, numData);
            dataB = GetData_RAM(B, numData);

            uint8 data = 0;
            uint8 limitUp = 0;
            uint8 limitDown = 0;

            for(int i = 0; i < numBytes; i++)
            {
                SET_LIMIT((uint8 *)dataA, (uint8 *)limitUpA_RAM, (uint8 *)limitDownA_RAM);
                SET_LIMIT((uint8 *)dataB, (uint8 *)limitUpB_RAM, (uint8 *)limitDownB_RAM);
            }

            if (NEED_FINISH_DRAW)
            {
                break;
            }
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::CalculateSums()
{
    DataSettings *ds = 0;
    uint16 *dataA = 0;
    uint16 *dataB = 0;

    GetDataFromEnd_RAM(0, &ds, &dataA, &dataB);

    int numPoints = NUM_BYTES(ds);

    int numAveragings = sDisplay_NumAverage();

    uint16 *sumA16 = (uint16 *)sumA_RAM;
    uint16 *sumB16 = (uint16 *)sumB_RAM;

    int iMax = numPoints / 2;

    for(int i = 0; i < iMax; i++)
    {
        uint16 data16 = dataA[i];
        *sumA16 = (uint8)data16;
        sumA16 += 2;
        *sumA16 = (uint8)(data16 >> 8);
        sumA16 += 2;

        data16 = dataB[i];
        *sumB16 = (uint8)data16;
        sumB16 += 2;
        *sumB16 = (uint8)(data16 >> 8);
        sumB16 += 2;
    }

    if(numAveragings > 1)
    {
        int numSameSettins = NumElementsWithSameSettings();
        if(numSameSettins < numAveragings)
        {
            numAveragings = numSameSettins;
        }

        for(int i = 1; i < numAveragings; i++)
        {
            GetDataFromEnd_RAM(i, &ds, &dataA, &dataB);

            sumA16 = (uint16 *)sumA_RAM;
            uint16 *dA = dataA;

            sumB16 = (uint16 *)sumB_RAM;
            uint16 *dB = dataB;

/* Прибавляем к младшим 16 разрядам суммы значение сигнала */
/* И записываем сумму на место 16 младших разрядов суммы */
/* Если значение выходит за 16 разрядов */
/* То производим полное вычисление суммы */
#define ALTERNATE_ADD(addr, shift)                                                                          \
    loSum = (uint)((*addr) + (uint8)(data16 >> shift));                                                     \
    *addr = (uint16)loSum;                                                                                  \
    if (loSum > 0xffffU)                                                                                    \
    {                                                                                                       \
        sum = (uint)((int)loSum - (int)((uint8)(data16 >> shift))) + ((*(addr + 1U)) >> 16) + (uint8)data16;\
        *addr = (uint16)sum;                                                                                \
        *(addr + 1) = (uint16)(sum >> 16);                                                                  \
    }                                                                                                       \
    addr += 2;

            for(int j = 0; j < iMax; j++)
            {
                uint sum = 0;
                uint loSum = 0;
                uint16 data16 = *dA++;     // Считываем первые два отсчёта данных

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4333)
#endif

                ALTERNATE_ADD(sumA16, 0U);  /** \todo Похоже, sum неправильно вычисляется, из-за чего артефаты при больших накоплениях. Нужно 
                                           (loSum - data16) заменить на старое значение (*data16) */

                ALTERNATE_ADD(sumA16, 8);

                data16 = *dB++;

                ALTERNATE_ADD(sumB16, 0);

                ALTERNATE_ADD(sumB16, 8);

#ifdef WIN32
#pragma warning(pop)
#endif

            }
        }
    }
    newSumCalculated[A] = newSumCalculated[B] = true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::AddData(uint8 *dataA, uint8 *dataB, DataSettings dss)
{
    if (!ENABLED_A(&dss) && !ENABLED_B(&dss))
    {
        return;
    }

    inFrameP2Pmode = false;

    TIME_TIME(&dss) = RTClock::GetPackedTime();

    CalculateLimits(dataA, dataB, &dss);

    PushData(&dss, dataA, dataB);

    CalculateSums();

    CalculateAroundAverage(dataA, dataB, &dss);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Storage::SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1)
{
    DataSettings* dp0 = DataSettingsFromEnd(elemFromEnd0);
    DataSettings* dp1 = DataSettingsFromEnd(elemFromEnd1);
    return DataSettingsIsEquals(dp0, dp1);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Storage::NumElementsWithSameSettings()
{
    int retValue = 0;
    for(retValue = 1; retValue < numElementsInStorage; retValue++)
    {
        if(!SettingsIsIdentical(retValue, retValue - 1))
        {
            break;
        }
    }
    return retValue;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Storage::NumElementsWithCurrentSettings()
{
    DataSettings dp;
    dp.Fill();
    int retValue = 0;
    int numElements = NumElementsInStorage();

    for(retValue = 0; retValue < numElements; retValue++)
    {
        if(!DataSettingsIsEquals(&dp, DataSettingsFromEnd(retValue)))
        {
            break;
        }
    }

    return retValue;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Storage::NumElementsInStorage()
{
    return numElementsInStorage;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
// Копирует данные канала ch из, определяемые ds, в одну из двух строк массива dataImportRel. Возвращаемое значение false означает, что данный канал 
// выключен.
static bool CopyData(DataSettings *ds, Channel ch, uint8 *dataImportRel)
{
    if((ch == A && !ENABLED_A(ds)) || (ch == B && !ENABLED_B(ds)))
    {
        return false;
    }

    uint8 *address = ADDRESS_DATA(ds);

    int length = NUM_BYTES(ds);

    if(ch == B && ENABLED_B(ds) && ENABLED_A(ds))
    {
        address += length;
    }

    RAM::MemCpy16(address, dataImportRel, length);

    return true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Storage::GetData_RAM(Channel ch, int fromEnd)
{
    uint8 *dataImport = (ch == A) ? RAM8(DS_DATA_IMPORT_REL_A) : RAM8(DS_DATA_IMPORT_REL_B);

    DataSettings* dp = DataSettingsFromEnd(fromEnd);

    if (dp == 0)
    {
        return 0;
    }

    if(CopyData(dp, ch, dataImport))
    {
        return dataImport;
    }

    return 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Storage::GetDataFromEnd(int fromEnd, DataSettings *ds, uint8 *dataA, uint8 *dataB)
{
    DataSettings *dataSettings = 0;
    uint16 *dA = 0;
    uint16 *dB = 0;
    
    if(GetDataFromEnd_RAM(fromEnd, &dataSettings, &dA, &dB))
    {
        memcpy(ds, dataSettings, sizeof(DataSettings));
        if(dA)
        {
            RAM::MemCpy16(dA, dataA, NUM_BYTES(ds));
        }
        if(dB)
        {
            RAM::MemCpy16(dB, dataB, NUM_BYTES(ds));
        }

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Storage::GetLimitation(Channel ch, uint8 *data, int direction)
{
    if (!MIN_MAX_ENABLED || NumElementsWithSameSettings() < 2)
    {
        return false;
    }

    DataSettings *ds = DataSettingsFromEnd(0);

    uint8 *limit = A == ch ? (uint8 *)limitDownA_RAM : (uint8 *)limitDownB_RAM;
    if (direction == 1)
    {
        limit = A == ch ? (uint8 *)limitUpA_RAM : (uint8 *)limitUpB_RAM;
    }
    RAM::MemCpy16(limit, data, NUM_BYTES(ds));

    return true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Storage::GetDataFromEnd_RAM(int fromEnd, DataSettings **ds, uint16 **dataA, uint16 **dataB)
{
    DataSettings *dp = DataSettingsFromEnd(fromEnd);

    if(dp == 0)
    {
        return false;
    }
    
    uint8 *dataImportRelA = RAM8(DS_DATA_IMPORT_REL_A);
    uint8 *dataImportRelB = RAM8(DS_DATA_IMPORT_REL_B);

    if(dataA != 0)
    {
        *dataA = CopyData(dp, A, dataImportRelA) ? (uint16 *)dataImportRelA : 0;
    }
    if(dataB != 0)
    {
        *dataB = CopyData(dp, B, dataImportRelB) ? (uint16 *)dataImportRelB : 0;
    }

    *ds = dp;

    return true;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Storage::GetAverageData(Channel ch)
{
    if (newSumCalculated[ch] == false)
    {
        return &gDataAve[ch][0];
    }

    newSumCalculated[ch] = false;

    DataSettings *ds = 0;
    uint16 *dA = 0;
    uint16 *dB = 0;
    GetDataFromEnd_RAM(0, &ds, &dA, &dB);

    if (ds == 0)
    {
        return 0;
    }

    int numPoints = NUM_BYTES(ds);

    if (sDisplay_GetModeAveraging() == Averaging_Around)
    {
        float *floatAveData = (ch == A) ? aveDataA_RAM : aveDataB_RAM;
        
        for (int i = 0; i < numPoints; i++)
        {
            gDataAve[ch][i] = (uint8)(floatAveData[i] + 0.5f);
        }
        return &gDataAve[ch][0];
    }

    int numAveraging = sDisplay_NumAverage();

    LIMIT_ABOVE(numAveraging, NumElementsWithSameSettings());

    for(int i = 0; i < numPoints; i++)
    {
        gDataAve[A][i] = (uint8)((float)RAM::ReadWord(sumA_RAM + i) / numAveraging + 0.5f);
        gDataAve[B][i] = (uint8)((float)RAM::ReadWord(sumB_RAM + i) / numAveraging + 0.5f);
    }

    return &gDataAve[ch][0];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Storage::NumberAvailableEntries()
{
    if(ADDRESS_DATA(&datas[iFirst]) == 0)
    {
        return 0;
    }

    int numElems = 0;
    LIMITATION_ABOVE(numElems, SIZE_POOL / SizeData(&datas[iLast]), NUM_DATAS);

    return numElems;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::NewFrameP2P(DataSettings *dss)
{
    if (!ENABLED_A(dss) && !ENABLED_B(dss))
    {
        return;
    }

    inFrameP2Pmode = true;
    dsP2P = *dss;
    dsP2P.addr = RAM8(DS_P2P_FRAME);
    RAM::MemClear(frameP2P, 2 * NUM_BYTES(dss));
    numPointsP2P = 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::AddPointsP2P(uint16 dataA, uint16 dataB)
{
    if (!ENABLED_A(&dsP2P) && !ENABLED_B(&dsP2P))
    {
        return;
    }

    FSMC_SET_MODE(ModeFSMC_RAM);

    int length = dsP2P.BytesInChannel();

    if (numPointsP2P >= length)                         // Если место во фрейме заполнено полностью
    {
        if (ENABLED_A(&dsP2P))                           // То сдвинем все точки во фрейме влево
        {
            RAM::MemShiftLeft(frameP2P + 2, length - 2, 2);
        }
        if (ENABLED_B(&dsP2P))
        {
            RAM::MemShiftLeft(frameP2P + 2 + length, length - 2, 2);
        }
    }

    int offsetWrite = (numPointsP2P >= length) ? length - 2 : numPointsP2P;

    if (ENABLED_A(&dsP2P))
    {
        uint16 *address = (uint16*)(frameP2P + offsetWrite);
        *address = dataA;
    }

    if (ENABLED_B(&dsP2P))
    {
        uint16 *address = (uint16 *)(frameP2P + length + offsetWrite);
        *address = dataB;
    }

    numPointsP2P += 2;

    FSMC_RESTORE_MODE();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Storage::GetFrameP2P_RAM(DataSettings **ds, uint8 **dataA, uint8 **dataB)
{
    if (!inFrameP2Pmode)
    {
        *ds = 0;
        *dataA = 0;
        *dataB = 0;
        return -1;
    }

    *ds = &dsP2P;
    *dataA = frameP2P;
    *dataB = frameP2P + dsP2P.BytesInChannel();

    return numPointsP2P;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *AddressChannel(DataSettings *ds, Channel ch)
{
    if (ch == A && ENABLED_A(ds))
    {
        return ADDRESS_DATA(ds);
    }

    if (ch == B && ENABLED_B(ds))
    {
        return ADDRESS_DATA(ds) + (ENABLED_A(ds) ? NUM_BYTES(ds) : 0);
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool DataSettingsIsEquals(const DataSettings *ds1, const DataSettings *ds2)
{
    /** @todo оптимизировать функцию сравнения */
    bool equals = (ENABLED_A(ds1) == ENABLED_A(ds2)) &&
        (ENABLED_B(ds1) == ENABLED_B(ds2)) &&
        (INVERSE_A(ds1) == INVERSE_A(ds2)) &&
        (INVERSE_B(ds1) == INVERSE_B(ds2)) &&
        (RANGE_A(ds1) == RANGE_A(ds2)) &&
        (RANGE_B(ds1) == RANGE_B(ds2)) &&
        (RSHIFT_A(ds1) == RSHIFT_A(ds2)) &&
        (RSHIFT_B(ds1) == RSHIFT_B(ds2)) &&
        (TBASE(ds1) == TBASE(ds2)) &&
        (TSHIFT(ds1) == TSHIFT(ds2)) &&
        (COUPLE_A(ds1) == COUPLE_A(ds2)) &&
        (COUPLE_B(ds1) == COUPLE_B(ds2)) &&
        (TRIGLEV_A(ds1) == TRIGLEV_A(ds2)) &&
        (TRIGLEV_B(ds1) == TRIGLEV_B(ds2)) &&
        (DIVIDER_A(ds1) == DIVIDER_A(ds2)) &&
        (DIVIDER_B(ds1) == DIVIDER_B(ds2)) &&
        (PEAKDET(ds1) == PEAKDET(ds2));

    return equals;
}
