#ifndef VECTOR_H
#define VECTOR_H
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    VECTOR_SUCCESS = 0,
    VECTOR_OUT_OF_MEMORY = 1,
    VECTOR_INVALID_ARGUMENT = 2,
    VECTOR_ERROR = 3,
    VECTOR_UNSUPPORTED_OPERATION = 4,
    VECTOR_NOT_IMPLEMENTED = 5,
    VECTOR_UNALIGNED_DATA = 6,
    VECTOR_SIZE_MISMATCH = 7
} vector_status_t;

typedef enum {
    DTYPE_INT8,
    DTYPE_INT16,
    DTYPE_INT32,
    DTYPE_FLOAT32,
} dtype;

static inline int sizeof_dtype(dtype type) {
    switch (type) {
        case DTYPE_INT8: return sizeof(int8_t);
        case DTYPE_INT16: return sizeof(int16_t);
        case DTYPE_INT32: return sizeof(int32_t);
        case DTYPE_FLOAT32: return sizeof(float);
        default: return -1; // Unsupported type
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
    void *data;  // Pointer to the data array *data must be 128-bit aligned and a multiple of 16 bytes*
    dtype type;  // Data type of the elements in the vector_t
    size_t size; // Current number of elements in the vector_t 
    bool owns_data; // Indicates if the vector_t owns the data (true if allocated with heap_caps_aligned_alloc)
} vector_t;

vector_t *vector_create(size_t size, dtype type);
vector_status_t vector_ok(vector_t *vec);
vector_status_t vector_destroy(vector_t *vec);
vector_status_t vec_add(const vector_t *vec1, const vector_t *vec2, vector_t *output_vec);
vector_status_t vec_sub(const vector_t *vec1, const vector_t *vec2, vector_t *output_vec);
vector_status_t vec_mul_shift(const vector_t *vec1, const vector_t *vec2, vector_t *output_vec, const int shift_amount);
vector_status_t vec_add_scalar(const vector_t *vec1, const int value, vector_t *output_vec);
vector_status_t vec_mul(const vector_t *vec1, const vector_t *vec2, vector_t *output_vec);
vector_status_t vec_sum(const vector_t *vec1, int32_t *output);
vector_status_t vec_mul_scalar_shift(const vector_t *vec1, const int val, vector_t *output_vec, const int shift_amount);
vector_status_t vec_dotp(const vector_t *vec1, const vector_t *vec2, int32_t *result);
vector_status_t vec_abs(const vector_t *vec1, vector_t* result);
vector_status_t vec_ceil(const vector_t *vec1, vector_t* result, const int ceiling);
vector_status_t vec_floor(const vector_t *vec1, vector_t* result, const int floor);
vector_status_t vec_neg(const vector_t *vec1, vector_t* result);
vector_status_t vec_relu(const vector_t *vec1, vector_t* result, const int multiplier, const int shift_amount);
vector_status_t vec_mac(const vector_t *vec1, int32_t* accumulator, const int multiplier);
vector_status_t vec_gt(const vector_t *vec1, const vector_t *vec2, vector_t *result);
vector_status_t vec_lt(const vector_t *vec1, const vector_t *vec2, vector_t *result);
vector_status_t vec_eq(const vector_t *vec1, const vector_t *vec2, vector_t *result);
vector_status_t vec_zeros(vector_t *vec1);
vector_status_t vec_ones(vector_t *vec1);
vector_status_t vec_fill(vector_t *vec1, const int val);
vector_status_t vec_xor(const vector_t *vec1, const vector_t *vec2, vector_t *result);
vector_status_t vec_and(const vector_t *vec1, const vector_t *vec2, vector_t *result);
vector_status_t vec_or(const vector_t *vec1, const vector_t *vec2, vector_t *result);
vector_status_t vec_not(const vector_t *vec1, vector_t *result);
vector_status_t vec_max(const vector_t *vec1, const vector_t *vec2, vector_t *result);
vector_status_t vec_min(const vector_t *vec1, const vector_t *vec2, vector_t *result);

#endif