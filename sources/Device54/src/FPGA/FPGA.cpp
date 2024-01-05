#include "FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Log.h"
#include "Data/Reader.h"
#include "Hardware/CPU.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/PageTime.h"
#include "Hardware/Panel.h"
#include "Utils/ProcessingSignal.h"
#include "Utils/Math.h"
#include "SCPI/SCPI.h"



#define FPGA_IN_PAUSE                   (bf.pause)
#define FPGA_CAN_READ_DATA              (bf.canRead)
#define FPGA_FIRST_AFTER_WRITE          (bf.firstAfterWrite)
#define NEED_STOP_AFTER_READ_FRAME_2P2  (bf.needStopAfterReadFrame2P2)


static struct BitFieldFPGA
{
    uint pause                     : 1;
    uint canRead                   : 1;
    uint firstAfterWrite           : 1;     // \brief ������������ � ������ �������������. ����� ������ ������ ��������� � ������� ����� �� 
                                            //        ������������ ������ ��������� ������ � ���, ������ ��� ��� �������� � ������ ������.
    uint needStopAfterReadFrame2P2 : 1;
    uint notUsed                   : 28;
} bf = {0, 1, 0, 0, 0};


#define NULL_TSHIFT 1000000

extern const int Kr[];

#define N_KR 100
const int Kr[] = {N_KR / 1, N_KR / 2, N_KR / 5, N_KR / 10, N_KR / 20};

StateWorkFPGA fpgaStateWork = StateWorkFPGA_Stop;
volatile static int numberMeasuresForGates = 1000;
static DataSettings ds;
static uint timeCompletePredTrig = 0;   // ����� ��������� ����� �����������. ���� == 0, �� ���������� �� ��������.
static uint8 *dataRandA = 0;
static uint8 *dataRandB = 0;
static uint timeStart = 0;
static uint timeSwitchingTrig = 0;
static bool readingPointP2P = false;    // ������� ����, ��� ����� � ���������� ���������� ����������� ������ ���������.
uint16 FPGA::adcValueFPGA = 0;
int FPGA::gRandStat[281];
int FPGA::addShiftForFPGA = 0;
static float gScaleRandStat = 0.0f;
bool gFPGAisCalibrateAddRshift = false;      // ���������� ��������� ���������� �������� � ������� �������� �������� � ��� ���� ��� ����� �������



static void InitADC();
/// �������� ������ � ������� �� ���� ����� ������
static void ShiftOnePoint2Right(uint8 *data, int size);


static uint16 READ_DATA_ADC_16(const uint16 *address, Channel ch )
{
    float delta = AVE_VALUE - (RShiftZero - SET_RSHIFT(ch)) / (RSHIFT_IN_CELL / 20.0f);
    BitSet16 point;
    BitSet16 retValue;
    point.halfWord = *address;
    int byte0 = (int)(((float)point.byte[0] - delta) * GetStretchADC(ch) + delta + 0.5f);
    LIMITATION(byte0, 0, 255);
    retValue.byte0 = (uint8)byte0;
    int byte1 = (int)(((float)point.byte[1] - delta) * GetStretchADC(ch) + delta + 0.5f);
    LIMITATION(byte1, 0, 255);
    retValue.byte1 = (uint8)byte1;
    return retValue.halfWord;
}


void FPGA::HardwareInit()
{
    __HAL_RCC_GPIOD_CLK_ENABLE();

    // �������� PD2 �� ������� ���������� EXTI2 - ���� ����� ��������� ���� ��������� ����� ��� ����������� ������
    GPIO_InitTypeDef isGPIOD =
    {
        GPIO_PIN_2,
        GPIO_MODE_IT_RISING,
        GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOD, &isGPIOD);

    // �������� ���������� ����� ������ �����, ����� �����. (FPGA_Start())
}

void FPGA::Init()
{
    Storage::Clear();
    HardwareInit();     // \todo ���� �� ���������� ������ ����� ������� �� ����������
    FreqMeter_Init();
    InitADC();
}


// ������� ����������, ����� ����� ��������� ��������� ������.
static void OnTimerCanReadData()
{
    FPGA_CAN_READ_DATA = 1;
}


void FPGA::SetENumSignalsInSec(int numSigInSec) 
{
    Timer::SetAndEnable(kENumSignalsInSec, OnTimerCanReadData, (uint)(1000.f / numSigInSec));
}


void FPGA::SwitchingTrig()
{
    if (TRIG_POLARITY_FRONT)
    {
        *WR_TRIG = 0;
        *WR_TRIG = 1;
    }
    else
    {
        *WR_TRIG = 1;
        *WR_TRIG = 0;
    }
    timeSwitchingTrig = TIME_MS;
    Panel::EnableLEDTrig(false);
}


uint16 FPGA::ReadFlag()
{
    uint16 flag = FSMC_READ(RD_FL);

    if (!RECORDER_MODE)
    {
        if(_GET_BIT(flag, FL_TRIG_READY) == 1 && timeStart > timeSwitchingTrig)
        {
            Panel::EnableLEDTrig(true);
        }
    }

    FreqMeter_Update(flag);

    return flag;
}


void FPGA::ReadPoint()
{
    readingPointP2P = false;
    ReadOnePoint();
}


bool FPGA::ReadOnePoint()
{
    if (readingPointP2P)
    {
        FSMC::RemoveFunctionAfterSetMode();
    }
    else if (FSMC::InSetStateMode()                  // ���� ���� ��������� � ��������� ����� ������
             || FSMC::GetMode() == ModeFSMC_Display) // ��� � ������ ������ � ��������
    {
        FSMC::SetFuncitonAfterSetMode(FPGA::ReadOnePoint);    // �� ��������� ����� ���� ������� ��� �������� ����� ������ ����
    }
    else
    {
        FSMC::RemoveFunctionAfterSetMode();

        FSMC_SET_MODE(ModeFSMC_FPGA);

        int16 balanceA = set.nrst_BalanceADC[A];
        int16 balanceB = set.nrst_BalanceADC[B];

        BitSet16 bsA;
        BitSet16 bsB;

        bsA.halfWord = READ_DATA_ADC_16(RD_ADC_A, A);
        bsB.halfWord = READ_DATA_ADC_16(RD_ADC_B, B);

        int16 byte0 = (int16)bsA.byte0 + balanceA;
        LIMITATION(byte0, 0, 255);
        bsA.byte0 = (uint8)byte0;
        LIMITATION(bsA.byte0, MIN_VALUE, MAX_VALUE);
        LIMITATION(bsA.byte1, MIN_VALUE, MAX_VALUE);
        byte0 = (int16)bsB.byte0 + balanceB;
        LIMITATION(byte0, 0, 255);
        bsB.byte0 = (uint8)byte0;
        LIMITATION(bsB.byte0, MIN_VALUE, MAX_VALUE);
        LIMITATION(bsB.byte1, MIN_VALUE, MAX_VALUE);

        Storage::AddPointsP2P(bsA.halfWord, bsB.halfWord);

        FSMC_RESTORE_MODE();

        readingPointP2P = true;
    }

    return readingPointP2P;
}


void FPGA::WriteStartToHardware()
{
    *WR_POST = gPost;
    *WR_PRED = (uint16)gPred;
    *WR_TRIG = TRIG_POLARITY_FRONT ? 1u : 0u;
    *WR_START = 0xffff;
}


void FPGA::Start()
{
    NEED_STOP_AFTER_READ_FRAME_2P2 = 0;

    WriteStartToHardware();

    timeCompletePredTrig = 0;

    ds.Fill();

    timeStart = TIME_MS;

    if (!IN_P2P_MODE)
    {
        HAL_NVIC_DisableIRQ(EXTI2_IRQn);    // ��������� ������ ��������� �����
    }
    else
    {
        HAL_NVIC_EnableIRQ(EXTI2_IRQn);     // �������� ������ ��������� �����
        Storage::NewFrameP2P(&ds);
    }

    fpgaStateWork = StateWorkFPGA_Work;
}


bool FPGA::CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{
    if (FPGA_FIRST_AFTER_WRITE)   // ���� ������ ������ ����� ������ � ������� -
    {
        FPGA_FIRST_AFTER_WRITE = 0;    // ���������� ���, ������ ��� ��� ����� ������� ��� ����������
        if (!START_MODE_SINGLE)         // � ���� �� ����������� ����� -
        {
            return false;               // �� ������� � �������
        }
    }

    if (rand < 500 || rand > 3500)
    {
        LOG_ERROR_TRACE("������!!! ������� %d", rand);
        return false;
    }

    static float minGate = 0.0f;
    static float maxGate = 0.0f;
    static int numElements = 0;
    static uint16 min = 0xffff;
    static uint16 max = 0;

    numElements++;
    if (rand < min)
    {
        min = rand;
    }
    if (rand > max)
    {
        max = rand;
    }

    static int stat[281];

    if (gScaleRandStat != 0.0f)
    {
        if (rand <= minGate)
        {
            stat[0]++;
        }
        else if (rand >= maxGate)
        {
            stat[280]++;
        }
        else
        {
            int step = (int)((rand - minGate) / gScaleRandStat);
            stat[step]++;
        }
    }

    if (minGate == 0.0f)
    {
        *eMin = min;
        *eMax = max;
        if (numElements < numberMeasuresForGates)
        {
            return true;
        }
        minGate = min;
        maxGate = max;
        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    if (numElements >= numberMeasuresForGates)
    {
        minGate = (9.0f * minGate + min) * 0.1f;
        maxGate = (9.0f * maxGate + max) * 0.1f;
        numElements = 0;
        min = 0xffff;
        max = 0;

        gScaleRandStat = (maxGate - minGate) / 281.0f;
        for (int i = 0; i < 281; i++)
        {
            gRandStat[i] = stat[i];
            stat[i] = 0;
        }
    }
    *eMin = (uint16)minGate;
    *eMax = (uint16)maxGate;

    if ((rand - 5) < minGate || (rand + 5) > maxGate)
    {
        return false;
    }

    return true;
}


int FPGA::CalculateShift()
{   /// \todo �� ������ ������������ �������
    uint16 min = 0;
    uint16 max = 0;

    if (!CalculateGate(adcValueFPGA, &min, &max))
    {
        return NULL_TSHIFT;
    }

    if (SHOW_RAND_INFO)
    {
        LOG_WRITE("rand = %d, ������ %d - %d", (int)adcValueFPGA, min, max);
    }

    if (IN_RANDOM_MODE)
    {
        float tin = (float)(adcValueFPGA - min) / (max - min) * 10e-9f;
        int retValue = (int)(tin / 10e-9f * Kr[SET_TBASE]);
        return retValue;
    }

    if (SET_TBASE == TBase_100ns && adcValueFPGA < (min + max) / 2)
    {
        return 0;
    }

    // \todo ������������ �� ������ ������� ��������������� ��������. �� PageDebug ����� ���������������, ����� ����������������� ��� �������������

    return -1;  // set.debug.altShift;
}


#define WRITE_AND_OR_INVERSE(addr, data, ch)                                                \
    if(SET_INVERSE(ch))                                                                     \
    {                                                                                       \
        data = (uint8)((int)(2 * AVE_VALUE) - LimitationUInt8(data, MIN_VALUE, MAX_VALUE)); \
    }                                                                                       \
    *(addr) = data;


#define BALANCE_DATA(pData, balance)        \
    if (nowBalance)                         \
    {                                       \
        int n = ((int)(*pData)) + balance;  \
        if (n < 0)                          \
        {                                   \
            n = 0;                          \
        }                                   \
        if (n > 255)                        \
        {                                   \
            n = 255;                        \
        }                                   \
        *pData = (uint8)n;                  \
     }                                      \
    nowBalance = !nowBalance;

static void ReadRandomizeChannel(Channel ch, uint16 addrFirstRead, uint8 *data, const uint8 *last, int step, int numSkipped)
{
    *WR_PRED = addrFirstRead;
    *WR_ADDR_NSTOP = 0xffff;

    uint16 *addr = ADDRESS_READ(ch);

    uint16 newData = 0;

    for (int i = 0; i < numSkipped; i++)
    {
        newData = *addr;
    }

    if (SET_INVERSE(ch))
    {
        while (data <= last)
        {
            newData = READ_DATA_ADC_16(addr, ch);
            *data = (uint8)((int)(2 * AVE_VALUE) - LimitationRet<uint8>((uint8)newData, MIN_VALUE, MAX_VALUE));
            data += step;
        }
    }
    else
    {
        while (data <= last)
        {
            *data = (uint8)READ_DATA_ADC_16(addr, ch);
            data += step;
        }
    }
}


bool FPGA::ReadRandomizeModeSave(bool first, bool last, bool onlySave)
{
    int bytesInChannel = ds.BytesInChannel();

    if (first)
    {
        memset(dataRandA, 0, (uint)bytesInChannel);
        memset(dataRandB, 0, (uint)bytesInChannel);
    }

    if (!onlySave)
    {
        int Tsm = CalculateShift();

        // ��������� �� �������� ��� ����� ���������� ���� ������
        // ������ dataRandA, dataRandB ������� ������� ��� ���������, �.�. � ������ ������������� � FPGA_Update() ����������� ��������� ������
        if (first)
        {
            if (ENABLED_DS_A)
            {
                memcpy(dataRandA, OUT_A, (uint)bytesInChannel);
            }
            if (ENABLED_DS_B)
            {
                memcpy(dataRandB, OUT_B, (uint)bytesInChannel);
            }
        }

        if (Tsm == NULL_TSHIFT)
        {
            return false;
        };

        if (START_MODE_SINGLE || SAMPLE_TYPE_IS_REAL)
        {
            ClearData();

            // ������� ������ ��� ������. ����� ������ ����� ����������� ������� ����� ����� ����, ��� ����� ��� �������������
            memset(dataRandA, 0, (uint)bytesInChannel);
            memset(dataRandB, 0, (uint)bytesInChannel);
        }

        // ������ ������� ������
        TBase tBase = SET_TBASE;
        int step = Kr[tBase];
#define NUM_ADD_STEPS 2
        int index = Tsm - addShiftForFPGA - NUM_ADD_STEPS * step;

        int numSkipped = 0;

        while (index < 0)
        {
            index += step;
            numSkipped++;
        }

        // addrFirstRead - �����, ������� �� ������ �������� � �������. ��� �����, � �������� ������� ����� ������ ������
        // �� ��������� ����� � ������ �� 3 ��������, ��� ���������. ������ ������ �����, ����� ��������� �� ����� �������, 
        // ������� �������� ��� ����������� ������.
        // ��������� ������ �� ������ ������� ������ ����� ��������� ����� �������� numSkipped
        uint16 addrFirstRead = (uint16)(*RD_ADDR_NSTOP + 16384 - (uint16)(bytesInChannel / step) - 1 - NUM_ADD_STEPS);

        //uint startRead = gTimerTics;

        ReadRandomizeChannel(B, addrFirstRead, &dataRandB[index], &dataRandB[FPGA_MAX_POINTS - 1], step, numSkipped);
        ReadRandomizeChannel(A, addrFirstRead, &dataRandA[index], &dataRandA[FPGA_MAX_POINTS - 1], step, numSkipped);
        
        if (START_MODE_SINGLE || SAMPLE_TYPE_IS_REAL)
        {
            Processing::InterpolationSinX_X(dataRandA, bytesInChannel, tBase);
            Processing::InterpolationSinX_X(dataRandB, bytesInChannel, tBase);
        }
    }

    // ������ ��������� ������ ������� �� ������� ���
    if (last)
    {
        CPU::RAM::MemCpy16(dataRandA, RAM8(FPGA_DATA_A), bytesInChannel);
        CPU::RAM::MemCpy16(dataRandB, RAM8(FPGA_DATA_B), bytesInChannel);
    }

    return true;
}


// balance - ����� ����� �����/���� ��� ������������
static void ReadChannel(uint8 *data, Channel ch, int length, uint16 nStop, bool shift, int balance)
{
    if (length == 0)
    {
        return;
    }
    *WR_PRED = nStop;
    *WR_ADDR_NSTOP = 0xffff;

    uint16 *p = (uint16 *)data;
    uint16 *endP = (uint16 *)&data[length];

    uint16 *address = ADDRESS_READ(ch);

    nStop = *address;

    if (shift)
    {
        *((uint8 *)p) = (uint8)(*address);

        p = (uint16 *)(((uint8 *)p) + 1);
        endP -= 8;                          // ��� �����, ���� �� ����� �� ������� ������ - ���� �� �������� ������ �� ���� ����
    }

    while (p < endP && FPGA_IN_PROCESS_OF_READ)
    {
        *p++ = READ_DATA_ADC_16(address, ch);
        *p++ = READ_DATA_ADC_16(address, ch);
        *p++ = READ_DATA_ADC_16(address, ch);
        *p++ = READ_DATA_ADC_16(address, ch);
        *p++ = READ_DATA_ADC_16(address, ch);
        *p++ = READ_DATA_ADC_16(address, ch);
        *p++ = READ_DATA_ADC_16(address, ch);
        *p++ = READ_DATA_ADC_16(address, ch);
    }

    if (shift)                              //  \todo ��-������, �������� ���� ����. ��-������, �� �����-�� ������� ��������
    {
        while (p < (uint16 *)&data[length - 1])
        {
            *p++ = READ_DATA_ADC_16(address, ch);
        }
    }

    if (balance != 0)
    {
        for (int i = shift ? 1 : 0; i < length; i += 2)
        {
            int newData = (int)data[i] + balance;
            if (newData < 0)
            {
                newData = 0;
            }
            if (newData > 255)
            {
                newData = 255;
            }
            data[i] = (uint8)newData;
        }
    }

    ShiftOnePoint2Right(data, length);
}


uint16 FPGA::ReadNStop()
{
    return (uint16)(*RD_ADDR_NSTOP + 16384 - (uint16)ds.BytesInChannel() / 2 - 1 - (uint16)gAddNStop);
}


void FPGA::ReadRealMode(uint8 *dataA, uint8 *dataB)
{
    FPGA_IN_PROCESS_OF_READ = 1;

    uint16 nStop = ReadNStop();

    bool shift = _GET_BIT(ReadFlag(), FL_LAST_RECOR) == 0;    // ���� true, ����� �������� ����� �� ����

    int balanceA = 0;
    int balanceB = 0;

    if (NRST_BALANCE_ADC_TYPE_IS_HAND && 
        SET_PEAKDET_DIS)               // ��� ���������� ������� ��������� ������������ �� �����
    {
        balanceA = NRST_BALANCE_ADC_A;
        balanceB = NRST_BALANCE_ADC_B;
    }

    ReadChannel(dataA, A, ds.BytesInChannel(), nStop, shift, balanceA);
    
    ReadChannel(dataB, B, ds.BytesInChannel(), nStop, shift, balanceB);

    CPU::RAM::MemCpy16(dataA, RAM8(FPGA_DATA_A), FPGA_MAX_POINTS);
    CPU::RAM::MemCpy16(dataB, RAM8(FPGA_DATA_B), FPGA_MAX_POINTS);

    FPGA_IN_PROCESS_OF_READ = 0;
}


static void ShiftOnePoint2Right(uint8 *data, int size)
{
    for (int i = size - 1; i >= 1; i--)
    {
        data[i] = data[i - 1];
    }
}


static void InverseDataIsNecessary(Channel ch, uint8 *data)
{
    if (SET_INVERSE(ch))
    {
        for (int i = 0; i < FPGA_MAX_POINTS; i++)
        {
            data[i] = (uint8)((int)(2 * AVE_VALUE) - LimitationRet<uint8>(data[i], MIN_VALUE, MAX_VALUE));
        }
    }
}


void FPGA::DataReadSave(bool first, bool saveToStorage, bool onlySave)
{
    // � ���� ������� ��������� ������, ��������������� ��� �������� ��������� �������, � �������� ���������� ������.

    FPGA_IN_PROCESS_OF_READ = 1;
    if (IN_RANDOM_MODE)
    {
        ReadRandomizeModeSave(first, saveToStorage, onlySave);
    }
    else
    {
        ReadRealMode(OUT_A, OUT_B);
    }

    int numBytes = ds.BytesInChannel();

    CPU::RAM::MemCpy16(RAM8(FPGA_DATA_A), OUT_A, numBytes);
    CPU::RAM::MemCpy16(RAM8(FPGA_DATA_B), OUT_B, numBytes);

    for (int i = 0; i < numBytes; i++)
    {
        LIMITATION(OUT_A[i], MIN_VALUE, MAX_VALUE);
        LIMITATION(OUT_B[i], MIN_VALUE, MAX_VALUE);
    }

    if (!IN_RANDOM_MODE)
    {
        InverseDataIsNecessary(A, OUT_A);
        InverseDataIsNecessary(B, OUT_B);
    }
    
    if (saveToStorage)
    {
        Storage::AddData(OUT_A, OUT_B, ds);
    }

    if (TRIG_MODE_FIND_AUTO)
    {
        FindAndSetTrigLevel();
    }

    FPGA_IN_PROCESS_OF_READ = 0;
}


bool FPGA::ProcessingData()
{
    bool retValue = false;                          // ����� ����� true, ����� ������ ���������

    static const int numRead[] = {100, 50, 20, 10, 5};

    int num = IN_RANDOM_MODE ? numRead[SET_TBASE] / 2 : 1;

    if (IN_RANDOM_MODE)
    {
        dataRandA = (uint8 *)AllocMemForChannelFromHeap(A, 0);
        dataRandB = (uint8 *)AllocMemForChannelFromHeap(B, 0);

        if (SAMPLE_TYPE_IS_REAL)
        {
            num = 1;                                // ��� �������� ������� ��������� ���������� ���������� - ��� ���� ������ ���� ���������.
        }
    }

    for (int i = 0; i < num; i++)
    {
        uint16 flag = ReadFlag();

        if (_GET_BIT(flag, FL_PRED_READY) == 0)       // ���� ���������� �� �������� - �������
        {
            break;
        }

        if (timeCompletePredTrig == 0)              // ���� ��������� ����������� ����� �� ���� �������������
        {
            timeCompletePredTrig = TIME_MS;         // ���������� �����, ����� ��� ���������.
        }

        if (i > 0)
        {
            uint time = TIME_MS;
            // � ��������������� ��������� ��� ��������� ����������� ����� ��������� ����� �������������
            while (_GET_BIT(flag, FL_TRIG_READY) == 0 && _GET_BIT(flag, FL_DATA_READY) == 0 && (TIME_MS - time) < 10)
            {                                                       // ��� ����� ��� ������ ������ ��������� �� �����
                flag = ReadFlag();
            }
            if (_GET_BIT(flag, FL_DATA_READY) == 0) 
            {
                i = num;
            }
        }

        if (_GET_BIT(flag, FL_TRIG_READY))                            // ���� ������ ������� �������������
        {
            if (_GET_BIT(flag, FL_DATA_READY) == 1)                   // ��������� ���������� ������
            {
                fpgaStateWork = StateWorkFPGA_Stop;                 // � ���������, ���� ������ ������
                HAL_NVIC_DisableIRQ(EXTI2_IRQn);                    // ��������� ������ �����
                DataReadSave(i == 0, i == num - 1, false);
                ProcessingAfterReadData();
                retValue = true;
            }
        }
        else if (START_MODE_AUTO)  // ���� �������� ������������� ����, � ������� �������������� ����� �������������
        {
            if (TIME_MS - timeCompletePredTrig > TSHIFT_2_ABS(2, SET_TBASE) * 1000) // ���� ������ ������ �������, ��� ���������� � ������ �������
            {
                if (IN_P2P_MODE)
                {
                    Panel::EnableLEDTrig(false);                                     // � ���������� ������ ������ ����� �������� �������������
                }
                else
                {
                    FPGA::SwitchingTrig();                                           // � ������������ ��� ������������� ��� ������������
                }
            }
        }

        if (i == num)
        {
            DataReadSave(false, true, true);
            retValue = true;
            break;
        }

        if (PANEL_CONTROL_RECEIVE && IN_RANDOM_MODE)
        {
            DataReadSave(false, true, true);
            retValue = true;
            break;
        }
    }

    SAFE_FREE(dataRandA);
    SAFE_FREE(dataRandB);

    return retValue;
}


void FPGA::ProcessingAfterReadData()
{
    if(!START_MODE_SINGLE)
    {
        if(IN_P2P_MODE && START_MODE_AUTO)                              // ���� ��������� � ������ ����������� ������ ��� �������������� �������������
        {
            if(!NEED_STOP_AFTER_READ_FRAME_2P2)
            {
                Timer::SetAndStartOnce(kTimerStartP2P, FPGA::Start, 1000);    // �� ����������� ��������� ������, ����� ������������� ������ �� ������
            }
        }
        else
        {
            Start();
        }
    }
    else
    {
        Panel::EnableLEDTrig(false);
    }
}


TBase FPGA::CalculateTBase(float freq)
{
    typedef struct
    {
        float freq;
        TBase tBase;
        uint8 notUsed0[3];
    } STR;
    
    static const STR structs[] =
    {
        {200e6f,    TBase_1ns,   {}},
        {100e6f,    TBase_2ns,   {}},
        {40e6f,     TBase_5ns,   {}},
        {20e6f,     TBase_10ns,  {}},
        {10e6f,     TBase_20ns,  {}},
        {3e6f,      TBase_50ns,  {}},
        {2e6f,      TBase_100ns, {}},
        {900e3f,    TBase_200ns, {}},
        {200e3f,    TBase_1us,   {}},
        {400e3f,    TBase_500ns, {}},
        {90e3f,     TBase_2us,   {}},
        {30e3f,     TBase_5us,   {}},
        {20e3f,     TBase_10us,  {}},
        {10e3f,     TBase_20us,  {}},
        {4e3f,      TBase_50us,  {}},
        {2e3f,      TBase_100us, {}},
        {1e3f,      TBase_200us, {}},
        {350.0f,    TBase_500us, {}},
        {200.0f,    TBase_1ms,   {}},
        {100.0f,    TBase_2ms,   {}},
        {40.0f,     TBase_5ms,   {}},
        {20.0f,     TBase_10ms,  {}},
        {10.0f,     TBase_20ms,  {}},
        {4.0f,      TBase_50ms,  {}},
        {2.0f,      TBase_100ms, {}},
        {0.0f,      TBaseSize,   {}}
    };


    const STR *str = &structs[0];
    while (str->freq != 0.0f)
    {
        if (freq >= str->freq)
        {
            return str->tBase;
        }
        ++str;
    }

    return TBase_200ms;
}


void FPGA::Update()
{
    if (SCPI::INPUT::needRunFPGA)
    {
        SCPI::INPUT::needRunFPGA = false;

        if (!IsRunning())
        {
            FPGA::OnPressStartStop();
        }
    }

    if (SCPI::INPUT::needStopFPGA)
    {
        SCPI::INPUT::needStopFPGA = false;

        if (IsRunning())
        {
            FPGA::OnPressStartStop();
        }
    }

    if (FPGA_IN_STATE_STOP)
    {
        return;
    }

    ReadFlag();

    if (gStateFPGA.needCalibration)              // ���� ����� � ����� ���������� -
    {
        ProcedureCalibration();            // ��������� �.
        gStateFPGA.needCalibration = false;
    }
    if (FPGA_IN_PAUSE)
    {
        return;
    }

	if(FPGA_NEED_AUTO_FIND)
    {
		AutoFind();
		return;
	}

    if(!FPGA_CAN_READ_DATA)
    {
        return;
    }

    if (!RECORDER_MODE)
    {
        ProcessingData();
    }

    FPGA_CAN_READ_DATA = 0;
}


void FPGA::OnPressStartStopInP2P()
{
    if(Timer::IsRun(kTimerStartP2P))             // ���� ��������� � ������ ����������� ������ � � ������ ������ ����� ����� ���������� ���������� 
    {                                           // ������� �������
        Timer::Disable(kTimerStartP2P);          // �� ������������� ������, ����� ����������� ������
    }
    else                                        // ���� ��� ������� ����� ����������
    {
        if(FPGA_IN_STATE_STOP)
        {
            Start();
        }
        else
        {   // �� ������������� ������� ����, ��� ����� ��������� �� ���� ��������� ��������� ����
            NEED_STOP_AFTER_READ_FRAME_2P2 = NEED_STOP_AFTER_READ_FRAME_2P2 == 0 ? 1u : 0u;
            Stop(false);
        }
    }
}


void FPGA::OnPressStartStop()
{
    if (!MODE_WORK_IS_DIR || CONSOLE_IN_PAUSE)           // ���� ��������� �� � ������ ����������������� ���������� �������
    {
        return;
    }

    if (IN_P2P_MODE)
    {
        OnPressStartStopInP2P();
    }
    else if(FPGA_IN_STATE_STOP) 
    {
        Start();
        fpgaStateWork = StateWorkFPGA_Wait;
    } 
    else
    {
        Stop(false);
        fpgaStateWork = StateWorkFPGA_Stop;
    } 
}


void FPGA::Stop(bool pause) 
{
    Panel::EnableLEDTrig(false);
    HAL_NVIC_DisableIRQ(EXTI2_IRQn);        // ��������� ���������� �� ������ ��������� �����
    fpgaStateWork = pause ? StateWorkFPGA_Pause : StateWorkFPGA_Stop;
}


void FPGA::Reset()
{
    bool needStart = FPGA_IS_RUNNING;
    Stop(false);
    if (needStart)
    {
        Start();
    }
}


bool FPGA::IsRunning()
{
    return !FPGA_IN_STATE_STOP;
}


void FPGA::ClearData()
{
    CPU::RAM::MemClear(RAM8(FPGA_DATA_A), FPGA_MAX_POINTS);
    CPU::RAM::MemClear(RAM8(FPGA_DATA_B), FPGA_MAX_POINTS);
}


void FPGA::SetNumberMeasuresForGates(int number)
{
    numberMeasuresForGates = number;
}


static void StopTemporaryPause()
{
    FPGA_IN_PAUSE = 0;
}


void FPGA::TemporaryPause()
{
    FPGA_IN_PAUSE = 1;
    Timer::SetAndStartOnce(kTemporaryPauseFPGA, StopTemporaryPause, 100);
}


void FPGA::FindAndSetTrigLevel()
{
    if(Storage::NumElementsInStorage() == 0 || TRIGSOURCE_IS_EXT)
    {
        return;
    }

    uint16 *dataA = 0;
    uint16 *dataB = 0;
    DataSettings *ds_ = 0;

    Storage::GetDataFromEnd_RAM(0, &ds_, &dataA, &dataB);

    const uint16 *data = TRIGSOURCE_IS_A ? dataA : dataB;

    int lastPoint = NUM_BYTES(ds_) - 1;

    uint8 min = Math::MinFromArray_RAM(data, 0, lastPoint);
    uint8 max = Math::MaxFromArray_RAM(data, 0, lastPoint);

    uint8 aveValue = (uint8)(((int)min + (int)max) / 2);

    const float scale = (float)(TrigLevMax - TrigLevZero) / (float)(MAX_VALUE - AVE_VALUE) / 2.4f;

    int trigLev = (int)(TrigLevZero + scale * ((int)aveValue - AVE_VALUE) - (SET_RSHIFT(TRIGSOURCE) - RShiftZero));

    SetTrigLev(TRIGSOURCE, (uint16)trigLev);
}


void FPGA::Write(TypeRecord type, uint16 *address, uint data, bool restart)
{
    // ���� ����������, ��������� ������������� ����� �� ����, ����� ����� ��� ������������
    ModeFSMC modePrev = FSMC::GetMode();
    bool needRestore = modePrev != ModeFSMC_FPGA;
    if (type == RecordFPGA && needRestore)
    {
        FSMC::SetMode(ModeFSMC_FPGA);
    }

    
    FPGA_FIRST_AFTER_WRITE = 1;
    if (restart)
    {
        if (FPGA_IN_PROCESS_OF_READ)
        {
            Stop(true);
            FPGA_IN_PROCESS_OF_READ = 0;
            Write(type, address, data);
            Start();
        }
        else
        {
            if (!FPGA_IN_STATE_STOP)
            {
                Stop(true);
                Write(type, address, data);
                Start();
            }
            else
            {
                Write(type, address, data);
            }
        }
    }
    else
    {
        Write(type, address, data);
    }


    // ��������������� ���������� ����� �� ����, ���� ����������.
    if (type == RecordFPGA && needRestore)
    {
        FSMC::SetMode(modePrev);
    }

    Panel::EnableLEDTrig(false); // ����� ������ ������� ��������� �������� �������������
}


static uint16 PinSelect(uint16 *addrAnalog)
{
    const uint16 pins[4] = {GPIO_PIN_3, GPIO_PIN_6, GPIO_PIN_10, GPIO_PIN_15};
    return pins[(int)addrAnalog];
}


static GPIO_TypeDef* AddrGPIO(uint16 *addrAnalog)
{
    GPIO_TypeDef *gpio[4] = {GPIOD, GPIOD, GPIOG, GPIOG};
    return gpio[(int)addrAnalog];
}


#define pinCLC      GPIO_PIN_10
#define pinData     GPIO_PIN_12
#define CHIP_SELECT_IN_LOW  HAL_GPIO_WritePin(AddrGPIO(addrAnalog), PinSelect(addrAnalog), GPIO_PIN_RESET);
#define CHIP_SELECT_IN_HI   HAL_GPIO_WritePin(AddrGPIO(addrAnalog), PinSelect(addrAnalog), GPIO_PIN_SET);
#define CLC_HI              HAL_GPIO_WritePin(GPIOC, pinCLC, GPIO_PIN_SET);
#define CLC_LOW             HAL_GPIO_WritePin(GPIOC, pinCLC, GPIO_PIN_RESET);
#define DATA_SET(x)         HAL_GPIO_WritePin(GPIOC, pinData, x);


void FPGA::Write(TypeRecord type, uint16 *address, uint data)
{
    if (type == RecordFPGA)
    {
        *address = (uint16)data;
    }
    else if (type == RecordAnalog)
    {
        uint16 *addrAnalog = address;
        CHIP_SELECT_IN_LOW
        for (int i = ((int)addrAnalog <= (int)CS2 ? 15 : 23); i >= 0; i--)      // ���� ������ ����� 16 ���, �� ������� 24 - ������ ������ ���� - 
        {                                                                       // ������ �� ��� ��������� �� ��, ��� ������������ ������
            DATA_SET((data & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            CLC_HI
            CLC_LOW
        }
        CHIP_SELECT_IN_HI;

        DATA_SET(GPIO_PIN_SET);
        CLC_HI
        CLC_LOW
    }
    else if (type == RecordDAC)
    {
        uint16 *addrAnalog = CS1;
        CHIP_SELECT_IN_LOW
        for (int i = 31; i >= 0; i--)
        {
            DATA_SET((data & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            CLC_HI
            CLC_LOW
        }
        CHIP_SELECT_IN_HI;
    }
}


static void InitADC()
{
    /*
    ��� ��� �������������
    ���� - ADC3 - 18 ADC3_IN4 - PF6
    �������� ������� 25���
    ����� ������ :
    -��������� �� 1 ����������� ������
    - ��������� ��������� �� ������ �������� �������(���������� �� 112 - EXT11 - PC11)
    */
    
#ifdef STM32F437xx

    __ADC3_CLK_ENABLE();

    static GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_6,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOF, &isGPIO);

    GPIO_InitTypeDef isGPIOC =
    {
        GPIO_PIN_11,
        GPIO_MODE_IT_RISING,
        GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOC, &isGPIOC);

    HAL_NVIC_SetPriority(ADC_IRQn, PRIORITY_FPGA_ADC);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    handleADC.Instance = ADC3;
    handleADC.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handleADC.Init.Resolution = ADC_RESOLUTION12b;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.ScanConvMode = DISABLE;
    handleADC.Init.EOCSelection = ENABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DMAContinuousRequests = DISABLE;
    handleADC.Init.NbrOfConversion = 1;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.NbrOfDiscConversion = 0;
    handleADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    handleADC.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;

    if (HAL_ADC_Init(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;

    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handleADC, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_ADC_Start_IT(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
#endif
}


void FPGA::SetTPos(TPos tPos)
{
    TPOS = tPos;
    PageTime::OnChanged_TPos(true);
}

/** @}
 */
