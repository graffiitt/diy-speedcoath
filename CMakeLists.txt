cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(speedcoatch C CXX ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

message("Adding ${PROJECT_NAME} C files: " )
file(GLOB CPPFILE "src/display_task/*.c")
foreach(source ${CPPFILE})
    message("  - " ${source})
endforeach(source)
 
message("Adding ${PROJECT_NAME} H files: " )
file(GLOB HFILE "src/display_task/*.h")
foreach(source ${HFILE})
    message("  - " ${source})
endforeach(source)
 
add_executable(${PROJECT_NAME}
src/main.c
src/battery.c
${CPPFILE}
${HFILE}
)

set(CONFIG_PATH_FREERTOS ${CMAKE_CURRENT_LIST_DIR}/configs)

add_subdirectory(bluetooth)
add_subdirectory(button_handler)
add_subdirectory(gps-ublox-n8m-rp2040/gps_ublox)
add_subdirectory(st7565-library-pico)

target_link_libraries(${PROJECT_NAME}
button_handler
gps_ublox
st7565
ble_tools
hardware_adc
hardware_rtc
)


target_include_directories(${PROJECT_NAME} PRIVATE

${CMAKE_CURRENT_LIST_DIR}/inc
${CMAKE_CURRENT_LIST_DIR}/configs
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
configNUMBER_OF_CORES=1
)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
