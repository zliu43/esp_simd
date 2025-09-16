# IDF sets this when you run: idf.py set-target esp32s3
if (NOT IDF_TARGET STREQUAL "esp32s3")
  message(FATAL_ERROR
    "Component 'my_s3_component' requires ESP32-S3 because it uses S3-only instructions.\n"
    "Current IDF_TARGET='${IDF_TARGET}'.\n"
    "Fix: run 'idf.py set-target esp32s3' (or configure with -DIDF_TARGET=esp32s3).")
endif()

# This wrapper exposes include/ and pulls *all* .c/.S from src/**
set(ESP_SIMD_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(ESP_SIMD_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(ESP_SIMD_TST_DIR "${CMAKE_CURRENT_LIST_DIR}/test")

file(GLOB_RECURSE ESP_SIMD_CS   "${ESP_SIMD_SRC_DIR}/*.c")
file(GLOB_RECURSE ESP_SIMD_ASMS "${ESP_SIMD_SRC_DIR}/*.S")
file(GLOB_RECURSE ESP_SIMD_TST_CS "${ESP_SIMD_TST_DIR}/*.c")

set(ESP_SIMD_SRCS ${ESP_SIMD_CS} ${ESP_SIMD_ASMS} ${ESP_SIMD_TST_CS}) 

idf_component_register(
    SRCS         ${ESP_SIMD_CS} ${ESP_SIMD_ASMS} ${ESP_SIMD_TST_CS}
    INCLUDE_DIRS
        "${ESP_SIMD_INC_DIR}"           
        "${ESP_SIMD_INC_DIR}/vector"    
        "${ESP_SIMD_TST_DIR}"    
    REQUIRES driver
)
