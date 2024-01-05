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


file(GLOB SRC           src/*.*)
file(GLOB DISPLAY       ../../Device54/src/Display/Display.*)
file(GLOB FLASHDRIVE    src/FlashDrive/*.*)
file(GLOB GUI           src/GUI/*.*)
file(GLOB GUI_VCP       src/GUI/VCP/*.*)
file(GLOB HARDWARE      src/Hardware/*.*)

file(GLOB RESOURCES     resources/*)

set_source_files_properties(resources/*
                            PROPERTIES LANGUAGE RC)

source_group(src        FILES ${SRC})
source_group(Display    FILES ${DISPLAY})
source_group(FlashDrive FILES ${FLASHDRIVE})
source_group(GUI        FILES ${GUI})
source_group(GUI/VCP    FILES ${GUI_VCP})
source_group(Hardware   FILES ${HARDWARE})


add_executable( ${PROJECT_NAME}
                ${SRC}
                ${DISPLAY}
                ${FLASHDRIVE}
                ${GUI}
                ${GUI_VCP}
                ${HARDWARE}
                )

target_link_libraries(${PROJECT_NAME} wxbase32ud wxmsw32ud_core)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase32ud_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw32ud_core_vc_custom.dll $(OutputPath) /Y)
