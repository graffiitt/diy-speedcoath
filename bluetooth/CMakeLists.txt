cmake_minimum_required (VERSION 3.10)
project(ble_tools)

message("Adding ${PROJECT_NAME} CPP files: " )
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
 
target_link_libraries(${PROJECT_NAME}
pico_stdlib
#pico_btstack_classic
pico_btstack_ble
pico_btstack_cyw43
pico_cyw43_arch_none
)
 