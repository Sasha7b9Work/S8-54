#pragma once
#include "Settings/commonSettings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsMath Settings Math
 *  @{
 */

#define SOURCE_FFT                  (set.math_SourceFFT)
#define SOURCE_FFT_A                (SOURCE_FFT == SourceFFT_ChannelA)
#define SOURCE_FFT_B                (SOURCE_FFT == SourceFFT_ChannelB)

#define WINDOW_FFT                  (set.math_WindowFFT)
#define WINDOW_FFT_IS_HAMMING       (WINDOW_FFT == WindowFFT_Hamming)
#define WINDOW_FFT_IS_BLACKMAN      (WINDOW_FFT == WindowFFT_Blackman)
#define WINDOW_FFT_IS_HANN          (WINDOW_FFT == WindowFFT_Hann)
#define MAX_DB_FFT                  (set.math_FFTmaxDB)
#define FFT_ENABLED                 (set.math_EnableFFT)
#define MATH_ENABLED                (FUNC_MODE_DRAW_IS_ENABLED || FFT_ENABLED)

#define SCALE_FFT                   (set.math_ScaleFFT)
#define SCALE_FFT_IS_LOG            (SCALE_FFT == ScaleFFT_Log)
#define SCALE_FFT_IS_LINEAR         (SCALE_FFT == ScaleFFT_Linear)

#define MATH_FUNC                   (set.math_Function)
#define MATH_FUNC_IS_MUL            (MATH_FUNC == Function_Mul)
#define MATH_FUNC_IS_SUM            (MATH_FUNC == Function_Sum)

#define FUNC_MODE_DRAW              (set.math_ModeDraw)
#define FUNC_MODE_DRAW_IS_ENABLED   (FUNC_MODE_DRAW != FuncModeDraw_Disable)
#define FUNC_MODE_DRAW_IS_SEPARATE  (FUNC_MODE_DRAW == FuncModeDraw_Separate)
#define FUNC_MODE_DRAW_IS_TOGETHER  (FUNC_MODE_DRAW == FuncModeDraw_Together)

#define FFT_POS_CURSOR(num)         (set.math_PosCur[num])
#define FFT_POS_CURSOR_0            (FFT_POS_CURSOR(0))
#define FFT_POS_CURSOR_1            (FFT_POS_CURSOR(1))

#define MATH_CURRENT_CUR            (set.math_CurrentCursor)
#define MATH_CURRENT_CUR_IS_0       (MATH_CURRENT_CUR == 0)

#define MATH_DIVIDER                (set.math_Divider)

#define MATH_MODE_REG_SET           (set.math_ModeRegSet)
#define MATH_MODE_REG_SET_IS_RSHIFT (MATH_MODE_REG_SET == ModeRegSet_RShift)
#define MATH_MODE_REG_SET_IS_RANGE  (MATH_MODE_REG_SET == ModeRegSet_Range)

/** @}  @}
 */
