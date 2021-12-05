project (Loader54)

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
                    
add_definitions(-DSTM32F437xx -DVS_KEIL -DS8_54)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4068)    # The compiler ignored an unrecognized pragma
add_compile_options(/wd4100)
add_compile_options(/wd4201)    # nonstandard extension used: nameless struct/union
add_compile_options(/wd4244)
add_compile_options(/wd4302)
add_compile_options(/wd4310)
add_compile_options(/wd4311)
add_compile_options(/wd4312)
add_compile_options(/wd4366)
add_compile_options(/wd4477)
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a base
                                # class default constructor is inaccessible or deleted
add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
add_compile_options(/wd4774)    # 'string' : format string expected in argument number is not a string literal
add_compile_options(/wd4820)    # '<anonymous-tag>': 'N' bytes padding added after data member
add_compile_options(/wd5027)    # 'type': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C
                                # function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5208)
add_compile_options(/wd5219)    # implicit conversion from 'xxx' to 'float', possible loss of data
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies


file(GLOB SRC                          src/*.cpp
                                       src/*.h
                            ../libs/common/Log.*)
file(GLOB DISPLAY                      src/Display/*.*
                            ../libs/common/Display/Colors.*
                            ../libs/common/Display/Painter.*)

source_group(src            FILES ${SRC})
source_group(Display        FILES ${DISPLAY})

add_library(${PROJECT_NAME} ${SRC} ${DISPLAY})

