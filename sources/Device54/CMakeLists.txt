project (Device54)

include_directories(src
                    src/libs/FatFS
                    src/libs/HAL
                    src/libs/LwIP
                    src/libs/USBD
                    src/libs/USBH
                    ../libs/common
                    ../libs/VS
                    ../libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    ../libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src
                    ../libs/STM32Cube_F4/Middlewares/Third_Party/LwIP/src/include
                    ../libs/STM32Cube_F4/Middlewares/Third_Party/LwIP/system)
                    
add_definitions(-DSTM32F437xx -DDEVICE -DVS_KEIL -DS8_54)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(
    /wd4005
    /wd4100
    /wd4201     # nonstandard extension used: nameless struct/union
    /wd4244
    /wd4302
    /wd4310
    /wd4311
    /wd4312
    /wd4366
    /wd4477
    /wd4514     # 'function' : unreferenced inline function has been removed
    /wd4623     # 'derived class' : default constructor was implicitly defined as deleted because a base
                # class default constructor is inaccessible or deleted
    /wd4626     # 'xxx': assignment operator was implicitly defined as deleted
    /wd4774     # 'string' : format string expected in argument number is not a string literal
    /wd4820     # '<anonymous-tag>': 'N' bytes padding added after data member
    /wd5027     # 'type': move assignment operator was implicitly defined as deleted
    /wd5039     # 'xxx': pointer or reference to potentially throwing function passed to extern C
                # function under -EHc.
    /wd5045     # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    /wd5208
    /wd5219     # implicit conversion from 'xxx' to 'float', possible loss of data
    /wd5220     # a non-static data member with a volatile qualified type no longer implies
)

file(GLOB SRC                        src/*.*
                          ../libs/common/Log.*)
file(GLOB DATA                       src/Data/*.*)
file(GLOB DISPLAY                    src/Display/*.*
                          ../libs/common/Display/Colors.*
                          ../libs/common/Display/Painter.*
                          ../libs/common/Display/PainterPIC24.*)
file(GLOB DISPLAY_FONT               src/Display/Font/*.*)
file(GLOB LAN                        src/LAN/*.*)
file(GLOB FLASHDRIVE                 src/FlashDrive/*.*)
file(GLOB FPGA                       src/FPGA/*.*)
file(GLOB HARDWARE                   src/Hardware/*.*
                          ../libs/common/Hardware/Timer.*)
file(GLOB MENU                       src/Menu/*.*
                          ../libs/common/Menu/*.*)
file(GLOB MENU_PAGES                 src/Menu/Pages/*.*
                          ../libs/common/Menu/Pages/*.*)
file(GLOB SCPI                       src/SCPI/*.*)
file(GLOB SCPI_FUNCTIONS             src/SCPI/Function/*.*)
file(GLOB SELFRECORDER               src/SelfRecorder/*.*)
file(GLOB SETTINGS                   src/Settings/*.*
                          ../libs/common/Settings/*.*)
file(GLOB UTILS                      src/Utils/*.*
                          ../libs/common/Utils/CommonFunctions.*)
file(GLOB LWIP                       src/libs/LwIP/*.*)
file(GLOB LIBS_USBD                  src/libs/USBD/*.*)

source_group(src            FILES ${SRC})
source_group(Data           FILES ${DATA})
source_group(Display        FILES ${DISPLAY})
source_group(Display/Font   FILES ${DISPLAY_FONT})
source_group(LAN            FILES ${LAN})
source_group(FlashDrive     FILES ${FLASHDRIVE})
source_group(FPGA           FILES ${FPGA})
source_group(Hardware       FILES ${HARDWARE})
source_group(Menu           FILES ${MENU})
source_group(Menu/Pages     FILES ${MENU_PAGES})
source_group(SCPI           FILES ${SCPI})
source_group(SCPI/Functions FILES ${SCPI_FUNCTIONS})
source_group(SelfRecorder   FILES ${SELFRECORDER})
source_group(Settings       FILES ${SETTINGS})
source_group(Utils          FILES ${UTILS})
source_group(libs/LwIP      FILES ${LWIP})
source_group(libs/USBD      FILES ${LIBS_USBD})

add_library(${PROJECT_NAME}
            ${SRC}
            ${DATA}
            ${DISPLAY}
            ${DISPLAY_FONT}
            ${LAN}
            ${FLASHDRIVE}
            ${FPGA}
            ${HARDWARE}
            ${MENU}
            ${MENU_PAGES}
            ${SCPI}
            ${SCPI_FUNCTIONS}
            ${SELFRECORDER}
            ${SETTINGS}
            ${UTILS}
            ${LWIP}
            ${LIBS_USBD}
)
