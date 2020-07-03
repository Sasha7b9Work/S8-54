cmake_minimum_required(VERSION 3.0)

project (S8-57)

include_directories(../../VS/Device/src
                    ../../Device/src
                    ../../Device/src/Hardware/FatFS
                    ../../Device/src/Hardware/HAL
                    ../../Device/src/Hardware/USBD
                    ../../Device/src/Hardware/USBH
                    ../..
                    ../../common/_VS/CMSIS
                    ../../common/_VS/FatFS
                    ../../common/_VS/HAL
                    ../../common/_VS/USBD
                    ../../common/_VS/USBH
                    ../../common/Display/Font
                    ../../../../ThirdParty/wxWidgets/include
                    ../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)
                    
add_definitions(-DSTM32F437xx -DWXUSINGDLL -DGUI -DDEVICE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4201)    # nonstandard extension used: nameless struct/union
add_compile_options(/wd4464)    # relative include path contains '..'
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4571)    # Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
add_compile_options(/wd4625)    # 'xxx': copy constructor was implicitly defined as deleted 
add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
add_compile_options(/wd4668)    # 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
add_compile_options(/wd4710)    # 'xxx': function not inlined
add_compile_options(/wd4774)    # 'string' : format string expected in argument number is not a string literal
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd4996)    # 'xxx': This function or variable may be unsafe.
add_compile_options(/wd5026)    # 'xxx': move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # 'type': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd26812)   # Prefer 'enum class' over 'enum' (Enum.3)

source_group(common                 FILES   ../../common/Command.h
                                            ../../common/Decoder_d.h
                                            ../../common/Decoder_d.cpp
                                            ../../common/Decoder_p.h
                                            ../../common/Message.h
                                            ../../common/Message.cpp)
source_group(_VS                                        REGULAR_EXPRESSION ../common/_VS/*)
source_group(_VS\\CMSIS                                 REGULAR_EXPRESSION ../common/_VS/CMSIS/*)
source_group(_VS\\FatFS                                 REGULAR_EXPRESSION ../common/_VS/FatFS/*)
source_group(_VS\\HAL                                   REGULAR_EXPRESSION ../common/_VS/HAL/*)
source_group(_VS\\HAL\\Legacy                           REGULAR_EXPRESSION ../common/_VS/HAL/Legacy/*)
source_group(_VS\\USBD                                  REGULAR_EXPRESSION ../common/_VS/USBD/*)
source_group(_VS\\USBH                                  REGULAR_EXPRESSION ../common/_VS/USBH/*)
source_group(FlashDrive                                 REGULAR_EXPRESSION src/FlashDrive/*)
source_group(FreqMeter                                  REGULAR_EXPRESSION src/FreqMeter/*)
source_group(GUI                                        REGULAR_EXPRESSION src/GUI/*)
source_group(GUI\\Dialogs                               REGULAR_EXPRESSION src/GUI/Dialogs/*)
source_group(Keyboard                                   REGULAR_EXPRESSION src/Keyboard/*)
source_group(Menu                                       REGULAR_EXPRESSION src/Menu/*)
source_group(Menu\\Pages                                REGULAR_EXPRESSION src/Menu/Pages/*)
source_group(Menu\\Pages\\Include                       REGULAR_EXPRESSION src/Menu/Pages/Include/*)
source_group(Menu\\Pages\\PageDebug                     REGULAR_EXPRESSION src/Menu/Pages/PageDebug/*)
source_group(Menu\\Pages\\PageDisplay                   REGULAR_EXPRESSION src/Menu/Pages/PageDisplay/*)
source_group(Menu\\Pages\\PageFunction                  REGULAR_EXPRESSION src/Menu/Pages/PageFunction/*)
source_group(Menu\\Pages\\PageRecorder                  REGULAR_EXPRESSION src/Menu/Pages/PageRecorder/*)
source_group(Menu\\Pages\\PageHelp                      REGULAR_EXPRESSION src/Menu/Pages/PageHelp/*)
source_group(Menu\\Pages\\PageMeasures                  REGULAR_EXPRESSION src/Menu/Pages/PageMeasures/*)
source_group(Menu\\Pages\\PageMemory                    REGULAR_EXPRESSION src/Menu/Pages/PageMemory/*)
source_group(Menu\\Pages\\PageService                   REGULAR_EXPRESSION src/Menu/Pages/PageService/*)
source_group(Menu\\Pages\\PageService\\PageDebug        REGULAR_EXPRESSION src/Menu/Pages/PageService/PageDebug/*)
source_group(Menu\\Pages\\PageTrig                      REGULAR_EXPRESSION src/Menu/Pages/PageTrig/*)
source_group(Multimeter                                 REGULAR_EXPRESSION src/Multimeter/*)
source_group(Osci\\Display                              REGULAR_EXPRESSION src/Osci/Display/*)
source_group(Osci\\Math                                 REGULAR_EXPRESSION src/Osci/Math/*)
source_group(Osci\\Measurements\\                       REGULAR_EXPRESSION src/Osci/Measurements/*)
source_group(SCPI                                       REGULAR_EXPRESSION src/SCPI/*)
source_group(Test                                       REGULAR_EXPRESSION src/Test/*)
source_group(Tester                                     REGULAR_EXPRESSION src/Tester/*)
source_group(Utils                                      REGULAR_EXPRESSION src/Utils/*)
source_group(src                FILES   ../../Device/src/defines.h
                                        ../../Device/src/defines.cpp
                                        ../../Device/src/device.h
                                        ../../Device/src/device.cpp
                                        ../../Device/src/globals.h
                                        ../../Device/src/globals.cpp
                                        ../../Device/src/log.h
                                                     src/log_win.cpp
                                                     src/main_win.cpp
                                        ../../Device/src/MessageMgr.h
                                        ../../Device/src/MessageMgr.cpp
                                        ../../Device/src/tables.h
                                        ../../Device/src/tables.cpp)
source_group(Display            FILES   ../../Device/src/Display/Colors.h
                                        ../../Device/src/Display/Colors.cpp
                                        ../../Device/src/Display/Console.h
                                        ../../Device/src/Display/Console.cpp
                                        ../../Device/src/Display/Display.h
                                        ../../Device/src/Display/Display.cpp
                                        ../../Device/src/Display/Primitives.h
                                        ../../Device/src/Display/Primitives.cpp
                                                     src/Display/Primitives_win.cpp
                                        ../../Device/src/Display/Grid.h
                                        ../../Device/src/Display/Grid.cpp
                                        ../../Device/src/Display/Painter.h
                                                     src/Display/Painter_win.cpp
                                        ../../Device/src/Display/PainterMem.h
                                        ../../Device/src/Display/PainterMem.cpp
                                        ../../Device/src/Display/Symbols.h
                                        ../../Device/src/Display/Warnings.h
                                        ../../Device/src/Display/Warnings.cpp)
source_group(Display\\Font      FILES   ../../common/Display/Font/AdvancedFont_d.h
                                        ../../common/Display/Font/AdvancedFont_d.cpp
                                        ../../common/Display/Font/font5.inc
                                        ../../common/Display/Font/font8.inc
                                        ../../common/Display/Font/Font_d.h
                                        ../../common/Display/Font/Font_d.cpp
                                        ../../Panel/src/Display/Font/Font_p.h
                                                    src/Display/Font/Font_p_win.cpp)
source_group(FPGA               FILES   ../../Device/src/FPGA/AD9286.h
                                        ../../Device/src/FPGA/AD9286.cpp
                                        ../../Device/src/FPGA/Calibrator.h
                                        ../../Device/src/FPGA/Calibrator.cpp
                                        ../../Device/src/FPGA/Context.cpp
                                        ../../Device/src/FPGA/Context.h
                                        ../../Device/src/FPGA/ContextFreqMeter.cpp
                                        ../../Device/src/FPGA/ContextFreqMeter.h
                                        ../../Device/src/FPGA/ContextRecorder.cpp
                                        ../../Device/src/FPGA/ContextRecorder.h
                                        ../../Device/src/FPGA/ContextTester.cpp
                                        ../../Device/src/FPGA/ContextTester.h
                                        ../../Device/src/FPGA/FPGA.cpp
                                        ../../Device/src/FPGA/FPGA.h
                                                     src/FPGA/FPGA_win.cpp
                                        ../../Device/src/FPGA/FPGA_HAL.h
                                        ../../Device/src/FPGA/FPGA_HAL.cpp
                                                     src/FPGA/FPGA_HAL_win.cpp
                                        ../../Device/src/FPGA/MathFPGA.cpp
                                        ../../Device/src/FPGA/MathFPGA.h
                                        ../../Device/src/FPGA/SettingsFPGA.cpp
                                        ../../Device/src/FPGA/TypesFPGA.h)
source_group(Hardware           FILES   ../../Device/src/Hardware/Battery.h
                                        ../../Device/src/Hardware/Battery.cpp
                                        ../../Device/src/Hardware/Beeper.h
                                        ../../Device/src/Hardware/Beeper.cpp
                                        ../../Device/src/Hardware/Clock.h
                                        ../../Device/src/Hardware/Clock.cpp
                                        ../../Device/src/Hardware/PowerBreaker.h
                                        ../../Device/src/Hardware/PowerBreaker.cpp
                                        ../../Device/src/Hardware/Timer.h
                                        ../../Device/src/Hardware/Timer.cpp
                                        ../../Device/src/Hardware/VCP.h
                                                     src/Hardware/VCP_win.cpp)
source_group(Hardware\\HAL      FILES   ../../Device/src/Hardware/HAL/HAL.h
                                        ../../Device/src/Hardware/HAL/HAL_ADC1.cpp
                                        ../../Device/src/Hardware/HAL/HAL_ADC3.cpp
                                        ../../Device/src/Hardware/HAL/HAL_CRC32.cpp
                                        ../../Device/src/Hardware/HAL/HAL_DAC1.cpp
                                                     src/Hardware/HAL/HAL_DAC2_win.cpp
                                                     src/Hardware/HAL/HAL_FSMC_win.cpp
                                                     src/Hardware/HAL/HAL_FSMC_PAN_win.cpp
                                                     src/Hardware/HAL/HAL_IWDG_win.cpp
                                        ../../Device/src/Hardware/HAL/HAL_MSP.cpp
                                        ../../Device/src/Hardware/HAL/HAL_NVIC.cpp
                                        ../../Device/src/Hardware/HAL/HAL_OTP.cpp
                                        ../../Device/src/Hardware/HAL/HAL_PIO.cpp
                                        ../../Device/src/Hardware/HAL/HAL_PIO.h
                                                     src/Hardware/HAL/HAL_ROM_win.cpp
                                        ../../Device/src/Hardware/HAL/HAL_RTC.cpp
                                                     src/Hardware/HAL/HAL_TIM2_win.cpp
                                                     src/Hardware/HAL/HAL_TIM3_win.cpp
                                        ../../Device/src/Hardware/HAL/HAL_USART3.cpp
                                                     src/Hardware/HAL/HAL_win.cpp)
source_group(Hardware\\Memory   FILES   ../../Device/src/Hardware/Memory/ExtRAM.h
                                        ../../Device/src/Hardware/Memory/ExtRAM.cpp
                                                     src/Hardware/Memory/Memory_win.cpp
                                        ../../Device/src/Hardware/Memory/IntRAM.h
                                        ../../Device/src/Hardware/Memory/IntRAM.cpp
                                        ../../Device/src/Hardware/Memory/RAM.h
                                        ../../Device/src/Hardware/Memory/RAM.cpp
                                        ../../Device/src/Hardware/Memory/ROM.h
                                        ../../Device/src/Hardware/Memory/ROM.cpp
                                        ../../Device/src/Hardware/Memory/ROM_Data.cpp
                                        ../../Device/src/Hardware/Memory/ROM_NRST.cpp
                                        ../../Device/src/Hardware/Memory/Sector.h
                                        ../../Device/src/Hardware/Memory/Sector.cpp
                                                     src/Hardware/Memory/Sector_win.cpp)
source_group(Osci               FILES   ../../Device/src/Osci/AutoSearcher.cpp
                                        ../../Device/src/Osci/DeviceSettings.h
                                        ../../Device/src/Osci/DeviceSettings.cpp
                                        ../../Device/src/Osci/Osci.h
                                        ../../Device/src/Osci/Osci.cpp
                                        ../../Device/src/Osci/ParametersOsci.h
                                        ../../Device/src/Osci/ParametersOsci.cpp
                                        ../../Device/src/Osci/Reader.h
                                        ../../Device/src/Osci/Reader.cpp
                                        ../../Device/src/Osci/Roller.cpp
                                                     src/Osci/Osci_win.h
                                                     src/Osci/Osci_win.cpp)
source_group(Recorder           FILES   ../../Device/src/Recorder/DisplayRecorder.h
                                        ../../Device/src/Recorder/DisplayRecorder.cpp
                                        ../../Device/src/Recorder/Recorder.h
                                        ../../Device/src/Recorder/Recorder.cpp
                                                     src/Recorder/Recorder_win.h
                                                     src/Recorder/Recorder_win.cpp
                                        ../../Device/src/Recorder/Sensor.h
                                        ../../Device/src/Recorder/Sensor.cpp
                                        ../../Device/src/Recorder/StorageRecorder.h
                                        ../../Device/src/Recorder/StorageRecorder.cpp)
source_group(Settings           FILES   ../../Device/src/Settings/Settings.h
                                        ../../Device/src/Settings/Settings.cpp
                                        ../../Device/src/Settings/Settings_default.cpp
                                        ../../Device/src/Settings/SettingsNRST.h
                                        ../../Device/src/Settings/SettingsNRST.cpp
                                                     src/Settings/Settings_win.cpp
                                        ../../Device/src/Settings/ParametersOsci.h
                                        ../../Device/src/Settings/ParametersOsci.cpp)

file(GLOB SOURCES   ../../common/_VS/*
                    ../../common/_VS/CMSIS/*
                    ../../common/_VS/FatFS/*
                    ../../common/_VS/HAL/*
                    ../../common/_VS/HAL/Legacy/*
                    ../../common/_VS/USBD/*
                    ../../common/_VS/USBH/*
                    
                    ../../common/Command.h
                    ../../common/Decoder_d.h
                    ../../common/Decoder_d.cpp
                    ../../common/Decoder_p.h
                    ../../common/Message.h
                    ../../common/Message.cpp
                    ../../common/Transceiver.h
                    ../../common/Transceiver.cpp
                    ../common/Transceiver_win.cpp
                    
                    ../../Device/src/Data/*
                    
                    ../../Device/src/Display/Colors.h
                    ../../Device/src/Display/Colors.cpp
                    ../../Device/src/Display/Console.h
                    ../../Device/src/Display/Console.cpp
                    ../../Device/src/Display/Display.h
                    ../../Device/src/Display/Display.cpp
                    ../../Device/src/Display/Primitives.h
                    ../../Device/src/Display/Primitives.cpp
                                 src/Display/Primitives_win.cpp
                    ../../Device/src/Display/Grid.h
                    ../../Device/src/Display/Grid.cpp
                    ../../Device/src/Display/Painter.h
                                 src/Display/Painter_win.cpp
                    ../../Device/src/Display/PainterMem.h
                    ../../Device/src/Display/PainterMem.cpp
                    ../../Device/src/Display/Symbols.h
                    ../../Device/src/Display/Warnings.h
                    ../../Device/src/Display/Warnings.cpp
                    
                    ../../common/Display/Font/AdvancedFont_d.h
                    ../../common/Display/Font/AdvancedFont_d.cpp
                    ../../common/Display/Font/font5.inc
                    ../../common/Display/Font/font8.inc
                    ../../common/Display/Font/Font_d.h
                    ../../common/Display/Font/Font_d.cpp
                    ../../Panel/src/Display/Font/Font_p.h
                                src/Display/Font/Font_p_win.cpp

                    ../../Device/src/FlashDrive/*
                    
                    ../../Device/src/FPGA/AD9286.h
                    ../../Device/src/FPGA/AD9286.cpp
                    ../../Device/src/FPGA/Calibrator.h
                    ../../Device/src/FPGA/Calibrator.cpp
                    ../../Device/src/FPGA/Context.cpp
                    ../../Device/src/FPGA/Context.h
                    ../../Device/src/FPGA/ContextFreqMeter.cpp
                    ../../Device/src/FPGA/ContextFreqMeter.h
                    ../../Device/src/FPGA/ContextRecorder.cpp
                    ../../Device/src/FPGA/ContextRecorder.h
                    ../../Device/src/FPGA/ContextTester.cpp
                    ../../Device/src/FPGA/ContextTester.h
                    ../../Device/src/FPGA/FPGA.cpp
                    ../../Device/src/FPGA/FPGA.h
                                 src/FPGA/FPGA_win.cpp
                    ../../Device/src/FPGA/FPGA_HAL.h
                    ../../Device/src/FPGA/FPGA_HAL.cpp
                                 src/FPGA/FPGA_HAL_win.cpp
                    ../../Device/src/FPGA/SettingsFPGA.cpp
                    ../../Device/src/FPGA/MathFPGA.cpp
                    ../../Device/src/FPGA/MathFPGA.h
                    ../../Device/src/FPGA/TypesFPGA.h
                    
                    ../../Device/src/FreqMeter/*
                    
                                 src/GUI/*
                                 src/GUI/Dialogs/*
                    
                    ../../Device/src/Hardware/Battery.h
                    ../../Device/src/Hardware/Battery.cpp
                    ../../Device/src/Hardware/Beeper.h
                    ../../Device/src/Hardware/Beeper.cpp
                    ../../Device/src/Hardware/Clock.h
                    ../../Device/src/Hardware/Clock.cpp
                    ../../Device/src/Hardware/PowerBreaker.h
                    ../../Device/src/Hardware/PowerBreaker.cpp
                    ../../Device/src/Hardware/Timer.h
                    ../../Device/src/Hardware/Timer.cpp
                    ../../Device/src/Hardware/VCP.h
                                 src/Hardware/VCP_win.cpp

                                 src/Hardware/HAL/HAL_win.cpp
                    ../../Device/src/Hardware/HAL/HAL.h
                    ../../Device/src/Hardware/HAL/HAL_ADC1.cpp
                    ../../Device/src/Hardware/HAL/HAL_ADC3.cpp
                    ../../Device/src/Hardware/HAL/HAL_CRC32.cpp
                    ../../Device/src/Hardware/HAL/HAL_DAC1.cpp
                                 src/Hardware/HAL/HAL_DAC2_win.cpp
                                 src/Hardware/HAL/HAL_FSMC_win.cpp
                                 src/Hardware/HAL/HAL_FSMC_PAN_win.cpp
                                 src/Hardware/HAL/HAL_IWDG_win.cpp
                    ../../Device/src/Hardware/HAL/HAL_MSP.cpp
                    ../../Device/src/Hardware/HAL/HAL_NVIC.cpp
                    ../../Device/src/Hardware/HAL/HAL_OTP.cpp
                    ../../Device/src/Hardware/HAL/HAL_PIO.cpp
                    ../../Device/src/Hardware/HAL/HAL_PIO.h
                                 src/Hardware/HAL/HAL_ROM_win.cpp
                    ../../Device/src/Hardware/HAL/HAL_RTC.cpp
                                 src/Hardware/HAL/HAL_TIM2_win.cpp
                                 src/Hardware/HAL/HAL_TIM3_win.cpp
                    ../../Device/src/Hardware/HAL/HAL_USART3.cpp
                    
                    ../../Device/src/Hardware/Memory/ExtRAM.h
                    ../../Device/src/Hardware/Memory/ExtRAM.cpp
                    ../../Device/src/Hardware/Memory/IntRAM.h
                    ../../Device/src/Hardware/Memory/IntRAM.cpp
                                 src/Hardware/Memory/Memory_win.cpp
                    ../../Device/src/Hardware/Memory/RAM.h
                    ../../Device/src/Hardware/Memory/RAM.cpp
                    ../../Device/src/Hardware/Memory/Reader.h
                    ../../Device/src/Hardware/Memory/Reader.cpp
                    ../../Device/src/Hardware/Memory/ROM.h
                    ../../Device/src/Hardware/Memory/ROM.cpp
                    ../../Device/src/Hardware/Memory/ROM_Data.cpp
                    ../../Device/src/Hardware/Memory/ROM_NRST.cpp
                    ../../Device/src/Hardware/Memory/Sector.h
                    ../../Device/src/Hardware/Memory/Sector.cpp
                                 src/Hardware/Memory/Sector_win.cpp
                    
                    ../../Device/src/Keyboard/*

                    ../../Device/src/Menu/*
                    ../../Device/src/Menu/Pages/*
                    ../../Device/src/Menu/Pages/Include/*
                    ../../Device/src/Menu/Pages/PageDebug/*
                    ../../Device/src/Menu/Pages/PageDisplay/*
                    ../../Device/src/Menu/Pages/PageFunction/*
                    ../../Device/src/Menu/Pages/PageRecorder/*
                    ../../Device/src/Menu/Pages/PageHelp/*
                    ../../Device/src/Menu/Pages/PageMeasures/*
                    ../../Device/src/Menu/Pages/PageMemory/*
                    ../../Device/src/Menu/Pages/PageService/*
                    ../../Device/src/Menu/Pages/PageService/PageDebug/*
                    ../../Device/src/Menu/Pages/PageTrig/*

                    ../../Device/src/Multimeter/*

                    ../../Device/src/Osci/Display/*
                    ../../Device/src/Osci/Math/*
                    ../../Device/src/Osci/Measurements/*

                    ../../Device/src/Osci/AutoSearcher.cpp
                    ../../Device/src/Osci/DeviceSettings.h
                    ../../Device/src/Osci/DeviceSettings.cpp
                    ../../Device/src/Osci/Osci.h
                    ../../Device/src/Osci/Osci.cpp
                    ../../Device/src/Osci/ParametersOsci.h
                    ../../Device/src/Osci/ParametersOsci.cpp
                    ../../Device/src/Osci/Reader.h
                    ../../Device/src/Osci/Reader.cpp
                    ../../Device/src/Osci/Roller.cpp
                                 src/Osci/Osci_win.h
                                 src/Osci/Osci_win.cpp

                    ../../Device/src/SCPI/*

                    ../../Device/src/Recorder/DisplayRecorder.h
                    ../../Device/src/Recorder/DisplayRecorder.cpp
                    ../../Device/src/Recorder/Recorder.h
                    ../../Device/src/Recorder/Recorder.cpp
                                 src/Recorder/Recorder_win.h
                                 src/Recorder/Recorder_win.cpp
                    ../../Device/src/Recorder/Sensor.h
                    ../../Device/src/Recorder/Sensor.cpp
                    ../../Device/src/Recorder/StorageRecorder.h
                    ../../Device/src/Recorder/StorageRecorder.cpp
                    
                    ../../Device/src/Settings/Settings.h
                    ../../Device/src/Settings/Settings.cpp
                    ../../Device/src/Settings/Settings_default.cpp
                    ../../Device/src/Settings/SettingsNRST.h
                    ../../Device/src/Settings/SettingsNRST.cpp
                                 src/Settings/Settings_win.cpp
                    ../../Device/src/Settings/ParametersOsci.h
                    ../../Device/src/Settings/ParametersOsci.cpp
                    
					../../Device/src/Test/*
                    ../../Device/src/Tester/*
                    ../../Device/src/Utils/*

                    ../../Device/src/defines.h
                    ../../Device/src/defines.cpp
                    ../../Device/src/device.h
                    ../../Device/src/device.cpp
                    ../../Device/src/globals.h
                    ../../Device/src/globals.cpp
                    ../../Device/src/log.h
                                 src/log_win.cpp
                                 src/main_win.cpp
                    ../../Device/src/MessageMgr.h
                    ../../Device/src/MessageMgr.cpp
                    ../../Device/src/tables.h
                    ../../Device/src/tables.cpp)
                    
add_executable(S8-57 ${SOURCES})

target_link_libraries(S8-57 ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(S8-57 ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(S8-57 WinMM.lib)
target_link_libraries(S8-57 version.lib)
target_link_libraries(S8-57 Imm32.Lib)
target_link_libraries(S8-57 Setupapi.lib)
