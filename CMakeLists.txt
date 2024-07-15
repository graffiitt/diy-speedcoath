cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

project(speedcoatch C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE Release)
pico_sdk_init()

message("Adding ${PROJECT_NAME} CPP files: " )
file(GLOB CPPFILE "*/*.cpp")
foreach(source ${CPPFILE})
    message("  - " ${source})
endforeach(source)
 
message("Adding ${PROJECT_NAME} H files: " )
file(GLOB HFILE "*/*.h")
foreach(source ${HFILE})
    message("  - " ${source})
endforeach(source)
 

add_executable(${PROJECT_NAME}
main.cpp
${CPPFILE}
${HFILE}
)

target_link_libraries(${PROJECT_NAME}
pico_stdlib
hardware_uart
hardware_spi
pico_btstack_ble
pico_btstack_cyw43
pico_cyw43_arch_none
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )

target_compile_definitions(${PROJECT_NAME} PRIVATE
    RUNNING_AS_CLIENT=1
)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
