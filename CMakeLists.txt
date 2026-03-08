cmake_minimum_required(VERSION 3.13)

add_library(dfrobot_max30102 INTERFACE)

target_sources(dfrobot_max30102 INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/src/dfrobot_max30102.c
)

target_include_directories(dfrobot_max30102 INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/include)

# Link hardware_i2c for the Pico SDK
target_link_libraries(dfrobot_max30102 INTERFACE 
    pico_stdlib 
    hardware_i2c
)