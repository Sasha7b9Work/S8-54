#pragma once
#include "LAN/TCP.h"


#define ENTER_ANALYSIS                                      \
    Word parameter;                                         \
    if (SU::GetWord((const char *)buffer, &parameter, 0)) { \
        uint8 value = GetValueFromMap(map, &parameter);     \
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



typedef struct 
{
    char        *name;
    pFuncVU8    func;
} StructCommand;



class SCPI
{
public:
    static void AddNewData(uint8 *buffer, uint length);
    static void Update();

    static void ProcessingCommand(const StructCommand *commands, uint8 *buffer);

    static bool FirstIsInt(uint8 *buffer, int *value, int min, int max);

private:

    static void ParseNewCommand(uint8 *buffer);   ///< \todo Временно. Потом доделать
};

void Process_DISPLAY(uint8 *buffer);
void Process_CHANNEL(uint8 *buffer);
void Process_TRIG(uint8 *buffer);
void Process_TBASE(uint8 *buffer);
