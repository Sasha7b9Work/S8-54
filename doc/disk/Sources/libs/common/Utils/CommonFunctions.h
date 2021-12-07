#pragma once
#include "defines.h"


inline void EmptyFuncVV() {}

inline void EmptyFuncVpV(void *) {}

inline void EmptyFuncVII(int, int) {}

inline void EmptyFuncVI(int) {}

inline bool EmptyFuncBV() { return true; }

inline void EmptyFuncVB(bool) {}

inline void EmptyFuncpVII(void *, int, int) {}

void LoggingFloatArray(float *data, int num);

void LoggingUint8Array(uint8 *data, int num);
