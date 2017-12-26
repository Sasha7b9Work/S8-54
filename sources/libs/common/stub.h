#pragma once


#ifndef DISPLAY_RUN_AFTER_DRAW                                                                                                    // Display/Display.h
#define                                     DISPLAY_RUN_AFTER_DRAW(func)
#endif
#ifndef SOCKET_SEND                                                                                                            // Ethernet/TcpSocket.h
#define                                     SOCKET_SEND(buffer, size)
#endif
#ifndef MIN_VALUE                                                                                                                  // FPGA/FPGATypes.h
#define                                     MIN_VALUE 3
#endif
#ifndef MAX_VALUE
#define                                     MAX_VALUE 125
#endif
#ifndef REG_SET_ACTIVE_ON_CURSORS                                                                                          // Menu/Pages/PageCursors.h
#define                                     REG_SET_ACTIVE_ON_CURSORS true
#endif
#ifndef INIT_PAGE_DISPLAY                                                                                                  // Menu/Pages/PageDisplay.h
#define                                     INIT_PAGE_DISPLAY()
#endif
#ifndef SHOW_STRING_NAVI_TEMP                                                                                            // Settings/SettingsDisplay.h
#define                                     SHOW_STRING_NAVI_TEMP   false
#endif
#ifndef BACKGROUND_BLACK
#define                                     BACKGROUND_BLACK true
#endif
#ifndef FUNC_MODE_DRAW_IS_ENABLED                                                                                           // Settings/SettingsMath.h
#define                                     FUNC_MODE_DRAW_IS_ENABLED true
#endif
#ifndef MATH_FUNC_IS_SUM
#define                                     MATH_FUNC_IS_SUM false
#endif
#ifndef MATH_FUNC_IS_MUL
#define                                     MATH_FUNC_IS_MUL false
#endif
#ifndef SOUND_VOLUME                                                                                                     // Settings/SettingsService.h
#define                                     SOUND_VOLUME 100
#endif
#ifndef COLOR_SCHEME_IS_WHITE_LETTERS
#define                                     COLOR_SCHEME_IS_WHITE_LETTERS true
#endif
#ifndef LANG
#define                                     LANG    (set.common.lang)
#endif
#ifndef LANG_RU
#define                                     LANG_RU (LANG == Russian)
#endif
#ifndef MathCh                                                                                                             // Settings/Settingstypes.h
#define                                     MathCh 0
#endif
#ifndef VCP_FLUSH                                                                                                                         // VCP/VCP.h
#define                                     VCP_FLUSH()
#endif
#ifndef VCP_SEND_DATA_SYNCH
#define                                     VCP_SEND_DATA_SYNCH(buffer, size)
#endif

