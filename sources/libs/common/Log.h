#pragma once   

#if defined(DEBUG) && !defined(MSVC)
#define LOG_WRITE(...)          Log::Write(TypeTrace_Info, __VA_ARGS__)
#define LOG_ERROR(...)          Log::Write(TypeTrace_Error, __VA_ARGS__)
#define LOG_WRITE_TRACE(...)    Log::Trace(TypeTrace_Info, __MODULE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR_TRACE(...)    Log::Trace(TypeTrace_Error, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
// Когда нужен вспомогательный буфер для преобразования числа в строку, можно пользоваться этой функцией
#define LOG_WRITE_BUF(...)      { char buffer[100]; Log_Write(TypeTrace_Info, __VA_ARGS__); }
#define LOG_FUNC_ENTER          Log::Write(TypeTrace_Info, "%s enter", __FUNCTION__);
#define LOG_FUNC_LEAVE          Log::Write(TypeTrace_Info, "%s leave", __FUNCTION__);
#define LOG_TRACE               Log::Write(TypeTrace_Info, "%s : %d", __FILE__, __LINE__);
#define ASSEERT(cond, ...)      if(cond)(LOG_ERROR_TRACE(__VA_ARGS__));
#define ASSERT_RET(cond, ...)   if(cond) {LOG_ERROR_TRACE(__VA_ARGS__); return; }
#elif defined(MSVC)
#define ASSERT_RET(cont, ...)
#define LOG_ERROR_TRACE(...)
#define LOG_WRITE(...)          Log::Write(TypeTrace_Info, __VA_ARGS__)
#define LOG_TRACE
#define LOG_ERROR(...)          Log::Write(TypeTrace_Error, __VA_ARGS__)
#else
#define LOG_WRITE(...)
#define LOG_ERROR(...)
#define LOG_WRITE_TRACE(...)
#define LOG_ERROR_TRACE(...)
#define LOG_WRITE_BUF(...)
#define LOG_FUNC_ENTER
#define LOG_FUNC_LEAVE
#define LOG_TRACE
#define ASSERT(const, ...)
#define ASSERT_RET(cont, ...)
#endif


typedef enum
{
    TypeTrace_Info,
    TypeTrace_Error
} TypeTrace;

class Log
{
public:
    static void Trace(TypeTrace type, const char *module, const char *func, int numLine, char *format, ...);
    static void Write(TypeTrace type, char *format, ...);
    static void DisconnectLoggerUSB();
    static void EnableLoggerUSB(bool enable);
    static int GetNumStrings();
};
