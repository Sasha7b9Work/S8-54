#pragma once


#ifndef VCP_FLUSH
#define VCP_FLUSH()
#endif

#ifndef VCP_SEND_DATA_SYNCH
#define VCP_SEND_DATA_SYNCH(buffer, size)
#endif

#ifndef MATH_FUNC_IS_SUM
#define MATH_FUNC_IS_SUM false
#endif

#ifndef MATH_FUNC_IS_MUL
#define MATH_FUNC_IS_MUL false
#endif

#ifndef LANG
#define LANG    (set.common.lang)
#endif

#ifndef LANG_RU
#define LANG_RU (LANG == Russian)
#endif

#ifndef FUNC_MODE_DRAW_IS_ENABLED
#define FUNC_MODE_DRAW_IS_ENABLED true
#endif

#ifndef COLOR_SCHEME_IS_WHITE_LETTERS
#define COLOR_SCHEME_IS_WHITE_LETTERS true
#endif

#ifndef SHOW_STRING_NAVI_TEMP
#define SHOW_STRING_NAVI_TEMP   false
#endif

#ifndef SOUND_VOLUME
#define SOUND_VOLUME 100
#endif

#ifndef REG_SET_ACTIVE_ON_CURSORS
#define REG_SET_ACTIVE_ON_CURSORS true
#endif

#ifndef INIT_PAGE_DISPLAY
#define INIT_PAGE_DISPLAY()
#endif
