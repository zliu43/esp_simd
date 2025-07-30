#include "vector.h"
#include "vector_functions.h" // For SIMD vector operations
#include <stdlib.h> // For malloc and free
#include "esp_heap_caps.h" // For heap_caps_aligned_alloc

vector_t *vector_create(size_t size, dtype type) {
    vector_t *vec = malloc(sizeof(vector_t));
    if (!vec) {
        return NULL; // Memory allocation failed
    }

    vec->data = heap_caps_aligned_alloc(16, size * sizeof_dtype(type), MALLOC_CAP_DEFAULT); // Allocate memory for the data array
    if (!vec->data) {
        free(vec); // Free the vector_t structure if data allocation fails
        return NULL;
    }

    vec->type = type;
    vec->size = size; // Initialize size to 0
    vec->owns_data = true; // Indicates that this vector_t owns the data

    return vec; // Return the created vector_t
}

vector_status_t vector_ok(vector_t *vec) {
    if (vec->data == NULL) {
        return VECTOR_OUT_OF_MEMORY; // Data pointer is NULL, indicating an out-of-memory condition
    }
    if ((uint32_t)vec->data & 0xF) {
        return VECTOR_UNALIGNED_DATA; // not 16 byte aligned
    }
    if (vec->type < DTYPE_INT8 || vec->type > DTYPE_FLOAT32) {
        return VECTOR_UNSUPPORTED_OPERATION; // Unsupported data type
    }
    return VECTOR_SUCCESS; // All checks passed, vector_t is valid
}

vector_status_t vector_destroy(vector_t *vec) {
    if (vec && vec->owns_data) {
        if (vec->data) {
            free(vec->data); 
        }
        free(vec);
    }
    return VECTOR_SUCCESS;
}

vector_status_t vec_add(const vector_t *vec1, const vector_t *vec2, vector_t *result) {
    if(vec1->size != vec2->size || vec1->size != result->size){
        return VECTOR_SIZE_MISMATCH;
    } 
    if(vec1->type == vec2->type) {
        switch (vec1->type) {
            case DTYPE_INT8: {
                simd_add_i8(vec1->data, vec2->data, result->data, vec1->size);
                return VECTOR_SUCCESS; // Return success after addition
            }
            case DTYPE_INT16: {
                simd_add_i16(vec1->data, vec2->data, result->data, vec1->size);
                return VECTOR_SUCCESS;  
            }
            case DTYPE_INT32: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            case DTYPE_FLOAT32: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            default:
                return VECTOR_UNSUPPORTED_OPERATION; // Unsupported type
        }
    }
    else{   
        return VECTOR_UNSUPPORTED_OPERATION; // Unsupported type
    } 
}

vector_status_t vec_add_scalar(const vector_t *vec1, const int value, vector_t *result) {
    assert(vec1->size == result->size);
    if(vec1->type == DTYPE_INT8) {
        assert(value >= -128 && value <= 127); // Ensure value is within int8_t range
        int8_t *val_ptr = (int8_t *)&value; // Cast int to int8_t pointer
        simd_add_scalar_i8(vec1->data, val_ptr, result->data, vec1->size); 
        return VECTOR_SUCCESS; // Return success after addition
    }
    return VECTOR_UNSUPPORTED_OPERATION; // Unsupported type
}
 
vector_status_t vec_sub(const vector_t *vec1, const vector_t *vec2, vector_t *result) {
    assert(vec1->size == vec2->size); // Ensure both vectors have the same size 
    assert(vec1->size == result->size); // Ensure output vector_t has the same size
    if(vec1->type == vec2->type) {
        switch (vec1->type) {
            case DTYPE_INT8: {
                simd_sub_i8(vec1->data, vec2->data, result->data, vec1->size);
                return VECTOR_SUCCESS; // Return success after subtraction
            }
            case DTYPE_INT16: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            case DTYPE_INT32: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            case DTYPE_FLOAT32: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            default:
                return VECTOR_UNSUPPORTED_OPERATION; // Unsupported type
        }
    }
    return VECTOR_UNSUPPORTED_OPERATION;
}

vector_status_t vec_mul_shift(const vector_t *vec1, const vector_t *vec2, vector_t *result, const int shift_amount) {
    assert(vec1->size == vec2->size); // Ensure both vectors have the same size 
    assert(vec1->size == result->size); // Ensure output vector_t has the same size
    assert(shift_amount >= 0 && shift_amount <= 8); // Ensure shift amount is within valid range for int8_t
    if(vec1->type == vec2->type) {
        switch (vec1->type) {
            case DTYPE_INT8: {
                simd_mul_i8_shift(vec1->data, vec2->data, result->data, shift_amount, vec1->size);
                return VECTOR_SUCCESS; // Return success after multiplication
            }
            case DTYPE_INT16: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            case DTYPE_INT32: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            case DTYPE_FLOAT32: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            default:
                return VECTOR_UNSUPPORTED_OPERATION; // Unsupported type
        }
    }
    return VECTOR_UNSUPPORTED_OPERATION;
}

vector_status_t vec_mul(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->size == vec2->size); // Ensure both vectors have the same size 
    assert(vec1->size == result->size); // Ensure output vector_t has the same size
    if (vec1->type == DTYPE_INT8 &&
        vec2->type == DTYPE_INT8 &&
        result->type == DTYPE_INT16) {
        simd_mul_i8_to_i16(vec1->data, vec2->data, result->data, vec1->size);
        return VECTOR_SUCCESS;
    }
    return VECTOR_UNSUPPORTED_OPERATION;
}

vector_status_t vec_sum(const vector_t *vec1, int32_t* result){
    switch (vec1->type){
        case(DTYPE_INT8): { 
            simd_sum_i8(vec1->data, result, vec1->size);
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_NOT_IMPLEMENTED;
    } 
}

vector_status_t vec_mul_scalar_shift(const vector_t *vec1, const int val, vector_t *result, const int shift_amount) { 
    assert(vec1->size == result->size); // Ensure output vector_t has the same size
    assert(shift_amount >= 0 && shift_amount <= 8); // Ensure shift amount is within valid range for int8_t
    if(vec1->type == result->type) {
        switch (vec1->type) {
            case DTYPE_INT8: {
                assert(val >= -128 && val <= 127); // Ensure value is within int8_t range
                int8_t *val_ptr = (int8_t *)&val; // Cast int to int8_t pointer
                simd_mul_scalar_i8(vec1->data, val_ptr, result->data, shift_amount, vec1->size);
                return VECTOR_SUCCESS; // Return success after multiplication
            }
            case DTYPE_INT16: {
                //assert(val >= -32768 && val <= 32767); // Ensure value is within int16_t range
                //int16_t *val_ptr = (int16_t *)&val; // Cast int to int16_t pointer
                //simd_mul_scalar_i16(vec1->data, val_ptr, result->data, shift_amount, vec1->size);
                //return VECTOR_SUCCESS; // Return success after multiplication
                return VECTOR_NOT_IMPLEMENTED;  
            }
            case DTYPE_INT32: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            case DTYPE_FLOAT32: {
                return VECTOR_NOT_IMPLEMENTED;  
            }
            default:
                return VECTOR_UNSUPPORTED_OPERATION; // Unsupported type
        }
    }
    return VECTOR_UNSUPPORTED_OPERATION;
}

vector_status_t vec_dotp(const vector_t *vec1, const vector_t *vec2, int32_t *result){ 
    assert(vec1->size == vec2->size);           // Ensure input vector_ts have the same size
    assert(vec1->type == vec2->type);
    switch (vec1->type){
        case (DTYPE_INT8): {
            simd_dotp_i8(vec1->data, vec2->data, result,vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_abs(const vector_t *vec1, vector_t* result){
    assert(vec1->size == result->size);           // Ensure input vector_ts have the same size
    assert(vec1->type == result->type);
    switch (vec1->type){
        case (DTYPE_INT8): {
            simd_abs_i8(vec1->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_ceil(const vector_t *vec1, vector_t* result, const int ceiling){
    assert(vec1->size == result->size);           // Ensure input vector_ts have the same size
    assert(vec1->type == result->type);
     switch (vec1->type){
        case (DTYPE_INT8): {
            assert(ceiling >= -128 && ceiling <= 127); // Ensure ceiling is within int8_t range
            int8_t *max_val = (int8_t *)&ceiling; // Cast int to int8_t pointer
            simd_ceil_i8(vec1->data, result->data, max_val, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:        
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_floor(const vector_t *vec1, vector_t* result, const int floor){
    assert(vec1->size == result->size);           // Ensure input vector_ts have the same size
    assert(vec1->type == result->type);
     switch (vec1->type){
        case (DTYPE_INT8): {
            assert(floor >= -128 && floor <= 127); // Ensure floor is within int8_t range
            int8_t *min_val = (int8_t *)&floor; // Cast int to int8_t pointer
            simd_floor_i8(vec1->data, result->data, min_val, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:        
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_neg(const vector_t *vec1, vector_t* result){
    assert(vec1->size == result->size);           // Ensure input vector_ts have the same size
    assert(vec1->type == result->type);
    switch (vec1->type){
        case (DTYPE_INT8): {
            simd_neg_i8(vec1->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_relu(const vector_t *vec1, vector_t* result, const int multiplier, const int shift_amount){
    assert(vec1->size == result->size);           // Ensure input vector_ts have the same size
    assert(vec1->type == result->type);
    assert(multiplier >= 0 && multiplier < 128);
    assert(shift_amount >= 0 && shift_amount < 32);
    switch (vec1->type){
        case (DTYPE_INT8): {
            simd_relu_i8(vec1->data, multiplier, shift_amount, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

/*
 *
 * @note mutliplier will be cast as the same type as the vector
 */
vector_status_t vec_mac(const vector_t *vec1, int32_t* accumulator, const int multiplier){ 
    switch (vec1->type){
        case (DTYPE_INT8): {
            int8_t multiplier_i8 = (int8_t)multiplier;
            simd_mac_i8(vec1->data, accumulator, &multiplier_i8, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_gt(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type);
    assert(vec1->size == vec2->size && vec1->size == result->size);

    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_compare_gt_i8(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_lt(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type);
    assert(vec1->size == vec2->size && vec1->size == result->size);

    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_compare_lt_i8(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_eq(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type);
    assert(vec1->size == vec2->size && vec1->size == result->size);

    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_compare_eq_i8(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_zeros(vector_t *vec1){
    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_zeros_i8(vec1->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_ones(vector_t *vec1){
    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_ones_i8(vec1->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_fill(vector_t *vec1, const int val){
    switch (vec1->type){
        case (DTYPE_INT8): { 
            assert(val >= -128 && val <= 127); // Ensure value is within int8_t range
            int8_t *val_ptr = (int8_t *)&val; // Cast int to int8_t pointer
            simd_fill_i8(vec1->data, val_ptr, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_xor(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type);
    assert(vec1->size == vec2->size && vec1->size == result->size);
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_xor_i8(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        case (DTYPE_INT16): { 
            simd_xor_i16(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_and(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type);
    assert(vec1->size == vec2->size && vec1->size == result->size);
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_and_i8(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        case (DTYPE_INT16): { 
            simd_and_i16(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_not(const vector_t *vec1, vector_t *result){
    assert(vec1->type == result->type);
    assert(vec1->size == result->size);

    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_not_i8(vec1->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        case (DTYPE_INT16): { 
            simd_not_i16(vec1->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_or(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type);
    assert(vec1->size == vec2->size && vec1->size == result->size);
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_or_i8(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        }; 
        case (DTYPE_INT16): { 
            simd_or_i16(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_max(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type);
    assert(vec1->size == vec2->size && vec1->size == result->size);
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_max_i8(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_min(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type);
    assert(vec1->size == vec2->size && vec1->size == result->size);
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            simd_min_i8(vec1->data, vec2->data, result->data, vec1->size);
            return VECTOR_SUCCESS;
        };
        default:
            return VECTOR_NOT_IMPLEMENTED;
    }
}

//