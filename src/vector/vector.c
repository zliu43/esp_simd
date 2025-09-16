#include "vector.h"
#include "simd_functions.h"                                                                 // For SIMD vector operations
#include <stdlib.h>  
#include "esp_heap_caps.h"  
#include "esp_log.h"

#define VECTOR_STACK_INIT(name, length, dtype_enum)                      \ 
    alignas(16) uint8_t name##_buf[(length) * sizeof_dtype(dtype_enum)]; \ 
    vector_t name = {                                                    \
        .data = (void *)(name##_buf),                                    \
        .type = (dtype_enum),                                            \
        .size = (length),                                                \
        .owns_data = false                                               \
    }


vector_t *vector_create(size_t size, dtype type) {
    if (type < DTYPE_INT8 || type > DTYPE_FLOAT32) { return NULL;}

    vector_t *vec = malloc(sizeof(vector_t));                                               // Returns NULL if size == 0
    if (!vec) { return NULL;}
 
    vec->data = heap_caps_aligned_alloc(16, size * sizeof_dtype(type), MALLOC_CAP_DEFAULT); // Allocate aligned memory for the data array
    if (!vec->data) {                                           
        free(vec);  
        return NULL;
    }

    vec->type = type;
    vec->size = size;  
    vec->owns_data = true;                                                                  // Indicates that this vector_t owns the data 
    return vec;                                                                             // Return the created vector_t
}

vector_status_t vector_ok(vector_t *vec) {
    if (!vec || !vec->data) { return VECTOR_NULL;}                                          // Vec or data pointer is NULL
    if ((uint32_t)vec->data & 0xF) { return VECTOR_UNALIGNED_DATA;}                         // Data not 128-bit aligned
    if (vec->type < DTYPE_INT8 || vec->type > DTYPE_FLOAT32) { return VECTOR_TYPE_MISMATCH;}// Invalid Type
    return VECTOR_SUCCESS;                                                                  
}

vector_status_t vector_destroy(vector_t *vec) { 
    if (vec && vec->owns_data) {
        if (vec->data) { 
            heap_caps_free(vec->data);
            vec->data = NULL;
        }
    }
    free(vec);  
    vec = NULL;
    return VECTOR_SUCCESS;
}

