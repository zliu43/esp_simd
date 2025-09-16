#include "vector_bitwise_functions.h"
#include "simd_functions.h"

vector_status_t vec_and(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_and_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_and_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_and_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return simd_and_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_not(const vector_t *vec1, vector_t *result){ 
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}
    if (vec1->size != result->size) { return VECTOR_SIZE_MISMATCH;}

    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_not_i8((int8_t*)(vec1->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_not_i16((int16_t*)(vec1->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_not_i32((int32_t*)(vec1->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return simd_not_i32((int32_t*)(vec1->data), (int32_t*)(result->data), vec1->size); 
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_or(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_or_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_or_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_or_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return simd_or_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_xor(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_xor_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_xor_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_xor_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return simd_xor_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        default:
            return VECTOR_ERROR;
    }
}