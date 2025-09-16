#include "vector_extra_functions.h"
#include "simd_functions.h"

vector_status_t vec_relu(const vector_t *vec1, vector_t* result, const int multiplier, const unsigned int shift_amount){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8): {
            if (multiplier < INT8_MIN || multiplier > INT8_MAX) { return VECTOR_INVALID_ARGUMENT;}
            if (shift_amount > 7) { return VECTOR_INVALID_ARGUMENT;}
            return simd_relu_i8((int8_t*)(vec1->data), multiplier, shift_amount, (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): {
            if (multiplier < INT16_MIN || multiplier > INT16_MAX) { return VECTOR_INVALID_ARGUMENT;}
            if (shift_amount > 15) { return VECTOR_INVALID_ARGUMENT;}
            return simd_relu_i16((int16_t*)(vec1->data), multiplier, shift_amount, (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): {
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        case (DTYPE_FLOAT32): { 
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_mul_widen(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type){ return VECTOR_TYPE_MISMATCH;}  
    if (vec1->type == DTYPE_INT8 && vec2->type == DTYPE_INT8 && result->type == DTYPE_INT16) { 
        return simd_mul_i8_to_i16((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int16_t*)(result->data), vec1->size);;
    } else if (vec1->type == DTYPE_INT16 && vec2->type == DTYPE_INT16 && result->type == DTYPE_INT32) { 
        return simd_mul_i16_to_i32((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int32_t*)(result->data), vec1->size);;
    }
    return VECTOR_TYPE_MISMATCH;
}

