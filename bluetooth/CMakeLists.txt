cmake_minimum_required (VERSION 3.12)
set(TARGET_NAME ble_tools)

message("Adding ${TARGET_NAME} CPP files: " )
file(GLOB CPPFILE "*.c")
foreach(source ${CPPFILE})
    message("  - " ${source})
endforeach(source)
 
message("Adding ${TARGET_NAME} H files: " )
file(GLOB HFILE "*.h")
foreach(source ${HFILE})
    message("  - " ${source})
endforeach(source)

add_library(${TARGET_NAME} STATIC
${CPPFILE}
${HFILE}
)
 
target_link_libraries(${TARGET_NAME} 
pico_stdlib
pico_btstack_ble
pico_btstack_cyw43
pico_cyw43_arch_sys_freertos
FreeRTOS-Kernel-Heap4

)

target_include_directories(${TARGET_NAME} PUBLIC
    ${CONFIG_PATH_FREERTOS} 
    ${CMAKE_CURRENT_LIST_DIR}/../configs
)
    
target_compile_definitions(${TARGET_NAME} PUBLIC
    CYW43_TASK_STACK_SIZE=1024
    CYW43_LWIP=0
    RUNNING_AS_CLIENT=1
    configNUMBER_OF_CORES=1
)