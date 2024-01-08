#include "defines.h"
#include "SCPI.h"
#include "SCPI/Functions/CommonSCPI.h"
#include "SCPI/Functions/DisplaySCPI.h"
#include "SCPI/Functions/ChannelSCPI.h"
#include "SCPI/Functions/TrigSCPI.h"
#include "SCPI/Functions/TimebaseSCPI.h"
#include "SCPI/Functions/MathSCPI.h"
#include "SCPI/Functions/AcquireSCPI.h"
#include "SCPI/Functions/CursorSCPI.h"
#include "SCPI/Functions/MeasureSCPI.h"
#include "SCPI/Functions/UtilitySCPI.h"
#include "SCPI/Functions/MemorySCPI.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "Utils/StringUtils.h"
#include <ctype.h>
#include <string.h>
#include "usbd_conf.h"
#include "globals.h"
#include "Utils/Buffer.h"
#include "Log.h"
#include <stdlib.h>


bool SCPI::INPUT::needRunFPGA = false;
bool SCPI::INPUT::needStopFPGA = false;
bool SCPI::INPUT::needReset = false;
bool SCPI::INPUT::needAutoscale = false;
bool SCPI::INPUT::needSendDataA = false;
bool SCPI::INPUT::needSendDataB = false;


namespace SCPI
{
    class BufferSCPI : public Buffer
    {
    public:
        BufferSCPI() : Buffer(100), pointer(0) {};

        // Добавляет новые полученные символы
        void AppendSymbols(uint8 *symbols, uint length)
        {
            if ((int)length + pointer < Size())
            {
                for (int i = 0; i < (int)length; i++)
                {
                    data[pointer++] = (uint8)toupper((char)symbols[i]);
                }
            }
        }

        // Возвращает указатель на следующий запрос, если таковой имеется
        uint8 *NextRequest()
        {
            RemoveFirstEmpty();

            for (int i = 0; i < pointer; i++)
            {
                uint8 byte = data[i];
                if (byte == 0 || byte == 0x0D || byte == 0x0A)
                {
                    data[i] = 0;
                    return data;
                }
            }

            return nullptr;
        }

        // Удаляет первые запрос в очереди, если таковой имеется
        void RemoveRequest()
        {
            uint8 *request = NextRequest();

            if (request)
            {
                int remove_bytes = (int)strlen((char *)request) + 1;     // Столько байт нужно удалить - число символов и завершающий ноль

                if (remove_bytes == pointer)
                {
                    pointer = 0;
                }
                else
                {
                    for (int i = 0; i < remove_bytes; i++)
                    {
                        data[i] = data[i + remove_bytes];
                    }
                    pointer -= remove_bytes;
                }
            }
        }

    private:
        int pointer;

        // Удалить незначащие символы в начале буфера
        void RemoveFirstEmpty()
        {
            int remove_bytes = 0;                       // Число байт, которые нужно удалить

            for (int i = 0; i < pointer; i++)
            {
                uint8 byte = data[i];
                if (byte == 0 || byte == 0x0d || byte == 0x0a)
                {
                    remove_bytes++;
                }
                else
                {
                    break;
                }
            }

            if (remove_bytes > 0)
            {
                int new_count = pointer - remove_bytes;         // После удаления столько байт останется

                for (int i = 0; i < new_count; i++)
                {
                    data[i] = data[i + remove_bytes];
                }

                pointer -= remove_bytes;
            }
        }
    };


    static BufferSCPI buffer;

    static int FindNumSymbolsInCommand(uint8 *buffer);

    static void ParseNewCommand(uint8 *buffer);   // \todo Временно. Потом доделать
}


void SCPI::AddNewData(uint8 *data, uint length)
{
    buffer.AppendSymbols(data, length);
}


void SCPI::Update()
{
    while (buffer.NextRequest())
    {
        uint8 *request = buffer.NextRequest();

        while (*request == ':')
        {
            request++;
        }

        ParseNewCommand(request);

        buffer.RemoveRequest();
    }
}



void SCPI::ParseNewCommand(uint8 *data)
{
    static const StructCommand commands[] =
    {
        { "*IDN?",       COMMON::IDN },
        { "*RST",        COMMON::RESET },
        { "RUN",         COMMON::RUN },
        { "STOP",        COMMON::STOP },
        { "KEY",         COMMON::KEY },
        { "GOVERNOR",    COMMON::GOVERNOR },
        { "REQUEST ?",   COMMON::REQUEST },

        { "CHANNEL1",    CHANNEL },
        { "CHAN1",       CHANNEL },

        { "CHANNEL2",    CHANNEL },
        { "CHAN2",       CHANNEL },

        { "MATHEMATICS", MATH },
        { "MATH",        MATH },

        { "TIMEBASE",    TIMEBASE },
        { "TIME" ,       TIMEBASE },

        { "TRIGGER",     TRIG },
        { "TRIG",        TRIG },

        { "DISPLAY",     DISPLAY },             // Вначале всегда идёт полное слово, потом сокращение.
        { "DISP",        DISPLAY },             // Это нужно для правильного парсинга.

        { "ACQuire",     ACQUIRE },
        { "ACQ",         ACQUIRE },

        { "CURSOR",      CURSOR },
        { "CURS",        CURSOR },

        { "MEASURE",     MEAS },
        { "MEAS",        MEAS },

        { "MEMORY",      MEMORY },
        { "MEM",         MEMORY },

        { "UTILITY",     UTIL },
        { "UTIL",        UTIL },

        { 0, 0 }
    };

    ProcessingCommand(commands, data);
}



void SCPI::ProcessingCommand(const StructCommand *commands, uint8 *data) 
{
    int sizeNameCommand = FindNumSymbolsInCommand(data);

    int numCommand = -1;

    char *name = 0;

    do 
    {
        numCommand++;   
        name = commands[numCommand].name;
    } while (name != 0 && (!EqualsStrings((char*)data, name, sizeNameCommand)));

    if (name != 0) 
    {
        commands[numCommand].func(data + sizeNameCommand + 1);
    }
    else
    {
        SCPI_SEND(":COMMAND ERROR");
    }
}


int SCPI::FindNumSymbolsInCommand(uint8 *data)
{
    int pos = 0;
    while ((data[pos] != ':') && (data[pos] != ' ') && (data[pos] != '\0'))
    {
        pos++;
    }
    return pos;
}


bool SCPI::FirstIsInt(uint8 *data, int *value, int min, int max)
{
    Word param;
    if (SU::GetWord((const char *)data, &param, 0))
    {
        char *n = (char *)malloc((uint)param.numSymbols + 1);
        memcpy(n, param.address, (uint)param.numSymbols);
        n[param.numSymbols] = '\0';
        bool res = String2Int(n, value) && *value >= min && *value <= max;
        free(n);
        return res;
    }
    return false;
}
