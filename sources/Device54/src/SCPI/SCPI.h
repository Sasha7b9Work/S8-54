#pragma once
#include "LAN/TCP.h"


#define ENTER_ANALYSIS                                  \
    Word parameter;                                     \
    if (SU::GetWord((pchar)buffer, &parameter, 0)) {    \
        uint8 value = GetValueFromMap(map, &parameter); \
        if (value < 255) {

#define LEAVE_ANALYSIS   }                              \
        else SCPI_SEND(":DATA ERROR")                   \
        }


#define SCPI_SEND(...)                                  \
    VCP::SendStringAsynch(__VA_ARGS__);                 \
    TCP::SendString(__VA_ARGS__);                       \

#define ENTER_PARSE_FUNC(funcName)                      \
void funcName(uint8 *buffer)                            \
{                                                       \
    static const StructCommand commands[] =             \
    {

#define LEAVE_PARSE_FUNC                                \
        {0, 0}                                          \
    };                                                  \
    SCPI::ProcessingCommand(commands, buffer);          \
}



struct StructCommand
{
    char        *name;
    pFuncVU8    func;
};



namespace SCPI
{
    void AddNewData(uint8 *buffer, uint length);
    void Update();
    void ProcessingCommand(const StructCommand *commands, uint8 *buffer);
    bool FirstIsInt(uint8 *buffer, int *value, int min, int max);

    namespace INPUT
    {
        extern bool needRunFPGA;        // ������� ����, ��� ����� ��������� ������� ����� ����������
        extern bool needStopFPGA;       // ������� ����, ��� ����� ����������� ������� ����� ����������
        extern bool needReset;          // ������� ����, ��� ����� �������� ���������
        extern bool needAutoscale;      // ������� ����, ��� ����� ����� ������
    };
};

void Process_CHANNEL(uint8 *);
void Process_TRIG(uint8 *);
void Process_TBASE(uint8 *);
