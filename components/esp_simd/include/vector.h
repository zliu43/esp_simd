#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

#ifdef __cplusplus
extern "C" {
#endif 

#if ESP_SIMD_ENABLE_BASIC
#include "vector/vector_basic_functions.h"
#endif

#if ESP_SIMD_ENABLE_BITWISE
#include "vector/vector_bitwise_functions.h"
#endif

#if ESP_SIMD_ENABLE_COMPARE
#include "vector/vector_compare_functions.h"
#endif

#if ESP_SIMD_ENABLE_EXTRA
#include "vector/vector_extra_functions.h"
#endif
 
// Macro to help initialize vector on stack.
#define VECTOR_STACK_INIT(name, length, dtype_enum)                      \
    alignas(16) uint8_t name##_buf[(length) * sizeof_dtype(dtype_enum)]; \
    vector_t name = {                                                    \
        .data = (void *)(name##_buf),                                    \
        .type = (dtype_enum),                                            \
        .size = (length),                                                \
        .owns_data = false                                               \
    }
 
typedef enum {
    VECTOR_SUCCESS = 0,
    VECTOR_NULL = 1,
    VECTOR_INVALID_ARGUMENT = 2,
    VECTOR_ERROR = 3,
    VECTOR_UNSUPPORTED_OPERATION = 4,
    VECTOR_NOT_IMPLEMENTED = 5,
    VECTOR_UNALIGNED_DATA = 6,
    VECTOR_SIZE_MISMATCH = 7,
    VECTOR_TYPE_MISMATCH = 8
} vector_status_t;

typedef enum {
    DTYPE_INT8,
    DTYPE_INT16,
    DTYPE_INT32,
    DTYPE_FLOAT32,
} dtype;
 
static inline size_t sizeof_dtype(dtype t) {
    switch (t) {
        case DTYPE_INT8:    return sizeof(int8_t);
        case DTYPE_INT16:   return sizeof(int16_t);
        case DTYPE_INT32:   return sizeof(int32_t);
        case DTYPE_FLOAT32: return sizeof(float);
        default:            return 0;
    }
}
  

/**
 * @brief Represents a fixed-size array of elements with a specified data type intended for SIMD operations.
 *
 * The vector_t structure holds metadata for an array of elements, including a pointer to the data,
 * the data type, and the number of elements. It supports both heap and stack allocation.
 * The data must be 128-bit aligned to enable SIMD operations
 */
typedef struct {
    void *data;             // Pointer to the data array *data must be 128-bit aligned and a multiple of 16 bytes*
    dtype type;             // Data type of the elements in the vector_t
    size_t size;            // Current number of elements in the vector_t  
    bool owns_data;         // Indicates if the vector_t owns the data (true if allocated with heap_caps_aligned_alloc)
} vector_t;
 
/**
 * @brief Create a heap-allocated vector with 16-byte aligned storage.
 *
 * Allocates a vector_t and its data buffer for @p size elements of @p type.
 * The data pointer is guaranteed to be 128-bit (16-byte) aligned.
 *
 * @param size  Number of elements.
 * @param type  Element dtype (DTYPE_INT8/INT16/INT32/FLOAT32).
 * @return Pointer to a newly created vector_t on success, or NULL on allocation failure.
 *
 * @note The returned vector owns its data (owns_data = true).
 * @warning Returns NULL when size == 0
 * @warning On allocation failure this returns NULL; check before use.
 */
vector_t *vector_create(size_t size, dtype type);

/**
 * @brief Validate a vector_t instance and its storage.
 *
 * Verifies non-NULL vector, non-NULL data, supported dtype, and 16-byte alignment.
 *
 * @param vec  Vector to check.
 * @retval VECTOR_SUCCESS           Vector is valid.
 * @retval VECTOR_NULL              @p vec is NULL or @p vec->data is NULL.
 * @retval VECTOR_UNALIGNED_DATA    @p vec->data is not 16-byte aligned.
 * @retval VECTOR_TYPE_MISMATCH     @p vec->type is invalid/unsupported.
 */
vector_status_t vector_ok(vector_t *vec);

/**
 * @brief Frees the data buffer of a vector_t if owns_data is true
 *
 * If @p vec->owns_data is true, frees the data buffer and sets it to NULL. 
 *
 * @param vec  Vector pointer (may be NULL).
 * @retval VECTOR_SUCCESS  Destruction completed (no-op if @p vec->owns_data is False).
 *
 * @post On return, the vector memory is released; 
 */
vector_status_t vector_free_data(vector_t *vec);

/**
 * @brief Destroy a vector_t and optionally its data buffer.
 *
 * If @p vec->owns_data is true, frees the data buffer and sets it to NULL.
 * Always frees the vector container and invalidates the pointer.
 *
 * @param vec  Vector to destroy (may be NULL).
 * @retval VECTOR_SUCCESS  Destruction completed (no-op if @p vec is NULL).
 *
 * @post On return, the vector memory is released; do not use @p vec afterwards.
 */
vector_status_t vector_destroy(vector_t *vec);

#ifdef __cplusplus
}
#endif
 