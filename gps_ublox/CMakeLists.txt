cmake_minimum_required (VERSION 3.8)
project(gps_ublox)

message("Adding ${PROJECT_NAME} C files: " )
file(GLOB CPPFILE "*.c")
foreach(source ${CPPFILE})
    message("  - " ${source})
endforeach(source)
 
message("Adding ${PROJECT_NAME} H files: " )
file(GLOB HFILE "*.h")
foreach(source ${HFILE})
    message("  - " ${source})
endforeach(source)
 

add_library(${PROJECT_NAME} STATIC
${CPPFILE}
${HFILE}
)
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/uart_rx.pio)

target_link_libraries(${PROJECT_NAME}
pico_stdlib
hardware_uart
pico_multicore
hardware_pio
pico_async_context_threadsafe_background
)
 