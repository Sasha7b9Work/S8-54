# 2024/01/05 13:25:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (S8-54)

set(CMAKE_UNITY_BUILD OFF)

include_directories(.
                    src
                    ../../Device54/src
                    ../../libs
                    ../../libs/common
                    ../../libs/VS_GUI
                    ../../VS/ThirdParty/wxWidgets/include 
                    ../../generated/ThirdParty/lib/vc_dll/mswud
)

link_directories(../../generated/ThirdParty/lib/vc_dll)

add_definitions(-DSTM32F437xx -DWXUSINGDLL -DGUI -DDEVICE -DS8_54)

if(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

    add_compile_options(
        /wd4062
        /wd4201     # nonstandard extension used : nameless struct/union
        /wd4127     # conditional expression is constant
        /wd4211     # nonstandard extension used : redefined extern to static
        /wd4244
        /wd4255     # 'function' : no function prototype given: converting '()' to '(void)'
        /wd4310     # cast truncates constant value
        /wd4355     # 'this' : used in base member initializer list
        /wd4464     # relative include path contains '..'
        /wd4505     # 'function': unreferenced function with internal linkage has been removed
        /wd4514     # 'function' : unreferenced inline function has been removed
        /wd4623     # 'derived class' : default constructor was implicitly defined as deleted because a base
                    # class default constructor is inaccessible or deleted
        /wd4625     # 'derived class' : copy constructor was implicitly defined as deleted because a base
                    # class copy constructor is inaccessible or deleted
        /wd4626     # 'xxx': assignment operator was implicitly defined as deleted
        /wd4668     # 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
        /wd4710     # 'xxx': function not inlined
        /wd4774     # 'string' : format string expected in argument number is not a string literal
        /wd4820     # 'xxx': '4' bytes padding added after data member 'yyy'
        /wd4996     # 'xxx': This function or variable may be unsafe.
        /wd5026     # 'type': move assignment operator was implicitly defined as deleted
        /wd5027     # 'type': move assignment operator was implicitly defined as deleted
        /wd5039     # 'xxx': pointer or reference to potentially throwing function passed to extern C
                    # function under -EHc.
        /wd5045     # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /wd5204
        /wd5219
        /wd5220     # a non-static data member with a volatile qualified type no longer implies that
                    # compiler generated copy/move constructors and copy/move assignment operators are not
                    # trivial
        /wd5264
        /wd26812    # the enum type 'xxx' is unscoped. Prefer 'enum class' over 'enum'
    )
endif()


file(GLOB SRC                              src/*.*
                            ../../Device54/src/Globals.*
                            ../../Device54/src/Tables.*
                             ../../libs/common/Log.*)
file(GLOB DATA              ../../Device54/src/Data/*.*)
file(GLOB DISPLAY           ../../Device54/src/Display/Display.*
                             ../../libs/common/Display/Grid.*
                             ../../libs/common/Display/Painter.*
                             ../../libs/common/Display/PainterText.*
                             ../../libs/common/Display/Colors.*
                             ../../libs/common/Display/DisplayPrimitives.*
                             ../../libs/common/Display/PainterTextSTM429.*
                             ../../libs/common/Display/PainterSTM429.*
                            ../../Device54/src/Display/PainterData.*)
file(GLOB DISPLAY_FONT      ../../Device54/src/Display/Font/*.*)
file(GLOB FLASHDRIVE                       src/FlashDrive/*.*)
file(GLOB FPGA                             src/FPGA/*.*
                            ../../Device54/src/FPGA/FPGA.*
                            ../../Device54/src/FPGA/FPGAextensions.*
                            ../../Device54/src/FPGA/FPGAMath.*
                            ../../Device54/src/FPGA/fpgaSettings.*
                            ../../Device54/src/FPGA/FPGATypes.*
                             ../../libs/common/FPGA/*.*)
file(GLOB GUI                              src/GUI/*.*)
file(GLOB GUI_VCP                          src/GUI/VCP/*.*)
file(GLOB HAL                ../../libs/VS_GUI/stm32f4xx_hal.*)
file(GLOB HARDWARE                         src/Hardware/*.*
                            ../../Device54/src/Hardware/Hardware.*
                            ../../Device54/src/Hardware/Panel.*
                            ../../Device54/src/Hardware/VCP.h
                            ../../Device54/src/Hardware/CPU_Ram.cpp
                            ../../Device54/src/Hardware/CPU_Rtc.cpp
                             ../../libs/common/Hardware/Timer.*
                             ../../libs/common/Hardware/Sound.*)
file(GLOB HARDWARE_STM32     ../../libs/common/Hardware/stm32/*.*)
file(GLOB HARDWARE_STM32_4XX ../../libs/common/Hardware/stm32/4XX/Timer4XX.*)
file(GLOB LAN                             src/LAN/*.*)
file(GLOB MENU              ../../libs/common/Menu/*.*)
file(GLOB MENU_PAGES       ../../Device54/src/Menu/Pages/*.*
                            ../../libs/common/Menu/Pages/*.*)
file(GLOB SCPI             ../../Device54/src/SCPI/*.*)
file(GLOB SCPI_FUNCTIONS   ../../Device54/src/SCPI/Functions/*.*)
file(GLOB SETTINGS         ../../Device54/src/Settings/*.*)
file(GLOB UTILS            ../../Device54/src/Utils/*.*
                            ../../libs/common/Utils/Math.*
                            ../../libs/common/Utils/Map.*
                            ../../libs/common/Utils/Measures.*)

file(GLOB RESOURCES     resources/*)

set_source_files_properties(resources/*
                            PROPERTIES LANGUAGE RC)

source_group(src                FILES ${SRC})
source_group(Data               FILES ${DATA})
source_group(Display            FILES ${DISPLAY})
source_group(Display/Font       FILES ${DISPLAY_FONT})
source_group(FlashDrive         FILES ${FLASHDRIVE})
source_group(FPGA               FILES ${FPGA})
source_group(GUI                FILES ${GUI})
source_group(GUI/VCP            FILES ${GUI_VCP})
source_group(HAL                FILES ${HAL})
source_group(Hardware           FILES ${HARDWARE})
source_group(Hardware/stm32     FILES ${HARDWARE_STM32})
source_group(Hardware/stm32/4XX FILES ${HARDWARE_STM32_4XX})
source_group(LAN                FILES ${LAN})
source_group(Menu               FILES ${MENU})
source_group(Menu/Pages         FILES ${MENU_PAGES})
source_group(SCPI               FILES ${SCPI})
source_group(SCPI/Functions     FILES ${SCPI_FUNCTIONS})
source_group(Settings           FILES ${SETTINGS})
source_group(Utils              FILES ${UTILS})


add_executable( ${PROJECT_NAME}
                ${SRC}
                ${DATA}
                ${DISPLAY}
                ${DISPLAY_FONT}
                ${FLASHDRIVE}
                ${FPGA}
                ${GUI}
                ${GUI_VCP}
                ${HAL}
                ${HARDWARE}
                ${HARDWARE_STM32}
                ${HARDWARE_STM32_4XX}
                ${LAN}
                ${MENU}
                ${MENU_PAGES}
                ${SCPI}
                ${SCPI_FUNCTIONS}
                ${SETTINGS}
                ${UTILS}
                )

target_link_libraries(${PROJECT_NAME} wxbase32ud wxmsw32ud_core)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase32ud_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw32ud_core_vc_custom.dll $(OutputPath) /Y)
