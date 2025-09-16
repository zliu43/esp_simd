#include "vector_compare_functions.h"
#include "simd_functions.h"


vector_status_t vec_max(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_max_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_max_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_max_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return VECTOR_NOT_IMPLEMENTED;
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_min(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 
    
    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_min_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_min_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_min_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return VECTOR_NOT_IMPLEMENTED;
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_gt(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 

    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_compare_gt_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_compare_gt_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_compare_gt_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return VECTOR_NOT_IMPLEMENTED;
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_lt(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 

    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_compare_lt_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_compare_lt_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_compare_lt_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return VECTOR_NOT_IMPLEMENTED;
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_eq(const vector_t *vec1, const vector_t *vec2, vector_t *result){ 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 

    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_compare_eq_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_compare_eq_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_compare_eq_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return VECTOR_NOT_IMPLEMENTED;
        }
        default:
            return VECTOR_ERROR;
    }
}