#ifndef SCALAR_BASIC_FUNCTIONS_H
#define SCALAR_BASIC_FUNCTIONS_H

#include "vector.h" 
#include <string.h>
 
vector_status_t scalar_and(const vector_t *vec1, const vector_t *vec2, vector_t *result) { 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: {
            int8_t* vec1_data = (int8_t*)(vec1->data);
            int8_t* vec2_data = (int8_t*)(vec2->data); 
            int8_t* result_data = (int8_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] & vec2_data[i];
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            int16_t* vec1_data = (int16_t*)(vec1->data);
            int16_t* vec2_data = (int16_t*)(vec2->data); 
            int16_t* result_data = (int16_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] & vec2_data[i];
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32: case DTYPE_FLOAT32: {
            int32_t* vec1_data = (int32_t*)(vec1->data);
            int32_t* vec2_data = (int32_t*)(vec2->data); 
            int32_t* result_data = (int32_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] & vec2_data[i];
            }
            return VECTOR_SUCCESS;
        } 
        default:
            return VECTOR_ERROR; 
    }   
}
  
vector_status_t scalar_or(const vector_t *vec1, const vector_t *vec2, vector_t *result) { 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: {
            int8_t* vec1_data = (int8_t*)(vec1->data);
            int8_t* vec2_data = (int8_t*)(vec2->data); 
            int8_t* result_data = (int8_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] | vec2_data[i];
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            int16_t* vec1_data = (int16_t*)(vec1->data);
            int16_t* vec2_data = (int16_t*)(vec2->data); 
            int16_t* result_data = (int16_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] | vec2_data[i];
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32: case DTYPE_FLOAT32: {
            int32_t* vec1_data = (int32_t*)(vec1->data);
            int32_t* vec2_data = (int32_t*)(vec2->data); 
            int32_t* result_data = (int32_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] | vec2_data[i];
            }
            return VECTOR_SUCCESS;
        } 
        default:
            return VECTOR_ERROR; 
    }   
}
 
vector_status_t scalar_xor(const vector_t *vec1, const vector_t *vec2, vector_t *result) { 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: {
            int8_t* vec1_data = (int8_t*)(vec1->data);
            int8_t* vec2_data = (int8_t*)(vec2->data); 
            int8_t* result_data = (int8_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] ^ vec2_data[i];
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            int16_t* vec1_data = (int16_t*)(vec1->data);
            int16_t* vec2_data = (int16_t*)(vec2->data); 
            int16_t* result_data = (int16_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] ^ vec2_data[i];
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32: case DTYPE_FLOAT32: {
            int32_t* vec1_data = (int32_t*)(vec1->data);
            int32_t* vec2_data = (int32_t*)(vec2->data); 
            int32_t* result_data = (int32_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] ^ vec2_data[i];
            }
            return VECTOR_SUCCESS;
        } 
        default:
            return VECTOR_ERROR; 
    }   
}

vector_status_t scalar_not(const vector_t *vec1,  vector_t *result) { 
    if ( vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if ( vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: {
            int8_t* vec1_data = (int8_t*)(vec1->data);  
            int8_t* result_data = (int8_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = ~vec1_data[i];
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            int16_t* vec1_data = (int16_t*)(vec1->data); 
            int16_t* result_data = (int16_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = ~vec1_data[i];
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32: case DTYPE_FLOAT32: {
            int32_t* vec1_data = (int32_t*)(vec1->data); 
            int32_t* result_data = (int32_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = ~vec1_data[i];
            }
            return VECTOR_SUCCESS;
        } 
        default:
            return VECTOR_ERROR; 
    }   
}

#endif