cmake_minimum_required (VERSION 3.8)
project(button_handler)

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

target_link_libraries(${PROJECT_NAME}
pico_stdlib
)

 