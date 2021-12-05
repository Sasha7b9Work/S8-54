#pragma once


#define va_list char *
#define va_start(x, y) x = (char *)y;
#define vsprintf(x, y, z)
#define va_end(x)
