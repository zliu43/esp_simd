file(GLOB VECTOR_FUNCTIONS_i8_SRCS "vector_functions/i8/*.S")
file(GLOB VECTOR_FUNCTIONS_i16_SRCS "vector_functions/i16/*.S")
file(GLOB SCALAR_FUNCTIONS_i8_SRCS "scalar_functions/i8/*.c")

idf_component_register(
    SRCS "vector.c" ${VECTOR_FUNCTIONS_i8_SRCS} ${VECTOR_FUNCTIONS_i16_SRCS} ${SCALAR_FUNCTIONS_i8_SRCS}
    INCLUDE_DIRS "." "testcases" "vector_functions" "vector_functions/i8" "vector_functions/i16" "scalar_functions" "scalar_functions/i8"
    REQUIRES esp_timer
)