#pragma once


#define SCPI_NODE(key, strct)                {key, strct,   nullptr, nullptr}
#define SCPI_LEAF(key, func, hint, funcHint) {key, nullptr, func,    hint,   funcHint}
#define SCPI_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::IsLineEnding(&t)) { SCPI::SendBadSymbols();
#define SCPI_EPILOG(t)  return t; } return nullptr;

#define SCPI_RUN_IF_END(func) if(end) { SCPI_PROLOG(end) func; SCPI_EPILOG(end) }

#define SCPI_REQUEST(func)                          \
    const char *end = SCPI::BeginWith(buffer, "?"); \
    SCPI_RUN_IF_END(func)

#define SCPI_PROCESS_ARRAY(names, func)             \
    for(int i = 0; i < names[i][0] != 0; i++)       \
    {                                               \
        end = SCPI::BeginWith(buffer, names[i]);    \
        SCPI_RUN_IF_END(func)                       \
    }                                               \
    return nullptr;

#define SCPI_EXIT_ERROR()   LOG_WRITE("Ошибка теста SCPI %s:%d", __FILE__, __LINE__); return false;

#define SCPI_APPEND_STRING(string) SCPI::AppendNewData(string.c_str(), std::strlen(string.c_str())); SCPI::Update()

#define SCPI_SEND_PARAMETER_DOES_NOT_EXIST() SCPI::SendAnswer("The parameter does not exist for the current signal")
