#ifndef SCALAR_BASIC_FUNCTIONS_H
#define SCALAR_BASIC_FUNCTIONS_H

#include "vector.h" 
#include <string.h>


vector_status_t scalar_add(const vector_t *vec1, const vector_t *vec2, vector_t *result) { 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: {
            int8_t* vec1_data = (int8_t*)(vec1->data);
            int8_t* vec2_data = (int8_t*)(vec2->data); 
            int8_t* result_data = (int8_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int intermediate = vec1_data[i] + vec2_data[i];
                intermediate = intermediate > INT8_MAX ? INT8_MAX : intermediate; 
                intermediate = intermediate < INT8_MIN ? INT8_MIN : intermediate; 
                result_data[i] = (int8_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            int16_t* vec1_data = (int16_t*)(vec1->data);
            int16_t* vec2_data = (int16_t*)(vec2->data); 
            int16_t* result_data = (int16_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int intermediate = vec1_data[i] + vec2_data[i];
                intermediate = intermediate > INT16_MAX ? INT16_MAX : intermediate; 
                intermediate = intermediate < INT16_MIN ? INT16_MIN : intermediate; 
                result_data[i] = (int16_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32: {
            int32_t* vec1_data = (int32_t*)(vec1->data);
            int32_t* vec2_data = (int32_t*)(vec2->data); 
            int32_t* result_data = (int32_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int64_t intermediate = (int64_t)vec1_data[i] + (int64_t)vec2_data[i];
                intermediate = intermediate > INT32_MAX ? INT32_MAX : intermediate; 
                intermediate = intermediate < INT32_MIN ? INT32_MIN : intermediate; 
                result_data[i] = (int32_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_FLOAT32: {
            float* vec1_data = (float*)(vec1->data);
            float* vec2_data = (float*)(vec2->data); 
            float* result_data = (float*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                float intermediate =  vec1_data[i] + vec2_data[i]; 
                result_data[i] = intermediate;
            }
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR; 
    }   
}

vector_status_t scalar_sub(const vector_t *vec1, const vector_t *vec2, vector_t *result) { 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: {
            int8_t* vec1_data = (int8_t*)(vec1->data);
            int8_t* vec2_data = (int8_t*)(vec2->data); 
            int8_t* result_data = (int8_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int intermediate = vec1_data[i] - vec2_data[i];
                intermediate = intermediate > INT8_MAX ? INT8_MAX : intermediate; 
                intermediate = intermediate < INT8_MIN ? INT8_MIN : intermediate; 
                result_data[i] = (int8_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            int16_t* vec1_data = (int16_t*)(vec1->data);
            int16_t* vec2_data = (int16_t*)(vec2->data); 
            int16_t* result_data = (int16_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int intermediate = vec1_data[i] - vec2_data[i];
                intermediate = intermediate > INT16_MAX ? INT16_MAX : intermediate; 
                intermediate = intermediate < INT16_MIN ? INT16_MIN : intermediate; 
                result_data[i] = (int16_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32: {
            int32_t* vec1_data = (int32_t*)(vec1->data);
            int32_t* vec2_data = (int32_t*)(vec2->data); 
            int32_t* result_data = (int32_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int64_t intermediate = (int64_t)vec1_data[i] - (int64_t)vec2_data[i];
                intermediate = intermediate > INT32_MAX ? INT32_MAX : intermediate; 
                intermediate = intermediate < INT32_MIN ? INT32_MIN : intermediate; 
                result_data[i] = (int32_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_FLOAT32: {
            float* vec1_data = (float*)(vec1->data);
            float* vec2_data = (float*)(vec2->data); 
            float* result_data = (float*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                float intermediate =  vec1_data[i] - vec2_data[i]; 
                result_data[i] = intermediate;
            }
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR; 
    }   
}

vector_status_t scalar_add_scalar(const vector_t *vec1, const int scalar, vector_t *result) { 
    if (vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: {
            int8_t* vec1_data = (int8_t*)(vec1->data); 
            int8_t* result_data = (int8_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int intermediate = vec1_data[i] + scalar;
                intermediate = intermediate > INT8_MAX ? INT8_MAX : intermediate; 
                intermediate = intermediate < INT8_MIN ? INT8_MIN : intermediate; 
                result_data[i] = (int8_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            int16_t* vec1_data = (int16_t*)(vec1->data); 
            int16_t* result_data = (int16_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int intermediate = vec1_data[i] + scalar;
                intermediate = intermediate > INT16_MAX ? INT16_MAX : intermediate; 
                intermediate = intermediate < INT16_MIN ? INT16_MIN : intermediate; 
                result_data[i] = (int16_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32: {
            int32_t* vec1_data = (int32_t*)(vec1->data); 
            int32_t* result_data = (int32_t*)(result->data); 
            for (int i = 0; i < vec1->size; i++){
                int64_t intermediate = (int64_t)vec1_data[i] + scalar;
                intermediate = intermediate > INT32_MAX ? INT32_MAX : intermediate; 
                intermediate = intermediate < INT32_MIN ? INT32_MIN : intermediate; 
                result_data[i] = (int32_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_FLOAT32: {
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:
            return VECTOR_ERROR; 
    }   
}

vector_status_t scalar_add_scalar_f32(const vector_t *vec1, const float scalar, vector_t *result) { 
    if (vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_INT16: return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_INT32: return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_FLOAT32: {
            float* vec1_data = (float*)(vec1->data); 
            float* result_data = (float*)(result->data); 
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] + scalar;
            }
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR; 
    }   
}
 
vector_status_t scalar_mul(const vector_t *vec1, const vector_t *vec2, vector_t *result, const unsigned int shift_amount) { 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type) {
        case DTYPE_INT8: { 
            if (shift_amount > 7) { return VECTOR_INVALID_ARGUMENT;}
            int8_t* vec1_data = (int8_t*)(vec1->data);
            int8_t* vec2_data = (int8_t*)(vec2->data);
            int8_t* result_data = (int8_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int a = vec1_data[i];
                int b = vec2_data[i];
                int intermediate = a * b;
                intermediate = intermediate >> shift_amount;
                result_data[i] = (int8_t)intermediate;
            }
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            if (shift_amount > 15) { return VECTOR_INVALID_ARGUMENT;}
            int16_t* vec1_data = (int16_t*)(vec1->data);
            int16_t* vec2_data = (int16_t*)(vec2->data);
            int16_t* result_data = (int16_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int a = vec1_data[i];
                int b = vec2_data[i];
                int intermediate = a * b;
                intermediate = intermediate >> shift_amount;
                result_data[i] = (int16_t)intermediate;
            } 
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32: { 
            if (shift_amount > 31) { return VECTOR_INVALID_ARGUMENT;}
            int32_t* vec1_data = (int32_t*)(vec1->data);
            int32_t* vec2_data = (int32_t*)(vec2->data);
            int32_t* result_data = (int32_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int64_t a = (int64_t)vec1_data[i];
                int64_t b = (int64_t)vec2_data[i];
                int64_t intermediate = a * b;
                intermediate = intermediate >> shift_amount;
                result_data[i] = (int32_t)intermediate;
            }  
            return VECTOR_SUCCESS;
        }
        case DTYPE_FLOAT32: { 
            float* vec1_data = (float*)(vec1->data);
            float* vec2_data = (float*)(vec2->data);
            float* result_data = (float*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                float a = vec1_data[i];
                float b = vec2_data[i]; 
                result_data[i] = a * b;
            }  
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR; 
    }  
}

vector_status_t scalar_sum(const vector_t *vec1, int32_t* result) {  
    int32_t output = 0;
    switch (vec1->type) {
        case DTYPE_INT8: {
            int8_t* vec1_data = (int8_t*)(vec1->data); 
            for (int i = 0; i < vec1->size; i++){ 
                output += (int)vec1_data[i];
            }
            *result = output;
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16: { 
            int16_t* vec1_data = (int16_t*)(vec1->data); 
            for (int i = 0; i < vec1->size; i++){ 
                output += (int)vec1_data[i];
            }
            *result = output;
            return VECTOR_SUCCESS;
        }
       case DTYPE_INT32: {
            const int32_t *vec1_data = (const int32_t *)vec1->data;
 
            int64_t lane1 = 0, lane2 = 0, lane3 = 0, lane4 = 0;
 
            int n = vec1->size & ~3;           
            for (int i = 0; i < n; i += 4) {
                lane1 += vec1_data[i + 0];
                lane2 += vec1_data[i + 1];
                lane3 += vec1_data[i + 2];
                lane4 += vec1_data[i + 3];
                if (lane1 < INT32_MIN) lane1 = INT32_MIN; else if (lane1 > INT32_MAX) lane1 = INT32_MAX;
                if (lane2 < INT32_MIN) lane2 = INT32_MIN; else if (lane2 > INT32_MAX) lane2 = INT32_MAX;
                if (lane3 < INT32_MIN) lane3 = INT32_MIN; else if (lane3 > INT32_MAX) lane3 = INT32_MAX;
                if (lane4 < INT32_MIN) lane4 = INT32_MIN; else if (lane4 > INT32_MAX) lane4 = INT32_MAX;
            }
 
 
            uint32_t wsum = 0;
            wsum += (uint32_t)(int32_t)lane1;
            wsum += (uint32_t)(int32_t)lane2;
            wsum += (uint32_t)(int32_t)lane3;
            wsum += (uint32_t)(int32_t)lane4;
 
            for (int i = n; i < vec1->size; ++i) {
                wsum += (uint32_t)vec1_data[i];
            }

            *result = (int32_t)wsum;
            return VECTOR_SUCCESS;
        }
        case DTYPE_FLOAT32: { 
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:
            return VECTOR_ERROR; 
    }   
}

vector_status_t scalar_sum_f32(const vector_t *vec1, float* result) {  
    float output = 0;
    switch (vec1->type) {
        case DTYPE_INT8: return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_INT16: return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_INT32: return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_FLOAT32: { 
            float* vec1_data = (float*)(vec1->data); 
            for (int i = 0; i < vec1->size; i++){ 
                output += (float)vec1_data[i];
            }
            *result = output;
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR; 
    }   
}

vector_status_t scalar_mul_scalar(const vector_t *vec1, const int val, vector_t *result, const unsigned int shift_amount) {
    if (vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type) {
        case DTYPE_INT8: {
            if (shift_amount > 7) { return VECTOR_INVALID_ARGUMENT;}

            int8_t *vec1_data = (int8_t *)vec1->data;
            int8_t *result_data = (int8_t *)result->data;
            int16_t val_i8 = (int16_t)val;

            for (int i = 0; i < vec1->size; i++) {
                int16_t intermediate = (int16_t)(vec1_data[i] * val_i8);
                result_data[i] = (int8_t)(intermediate >> shift_amount);
            }
            return VECTOR_SUCCESS;
        }

        case DTYPE_INT16: {
            if (shift_amount > 15) { return VECTOR_INVALID_ARGUMENT;}

            int16_t *vec1_data = (int16_t *)vec1->data;
            int16_t *result_data = (int16_t *)result->data;
            int32_t val_i16 = (int32_t)val;

            for (int i = 0; i < vec1->size; i++) {
                int32_t intermediate = (int32_t)(vec1_data[i] * val_i16);
                result_data[i] = (int16_t)(intermediate >> shift_amount);
            }
            return VECTOR_SUCCESS;
        }

        case DTYPE_INT32: {
            if (shift_amount > 31) { return VECTOR_INVALID_ARGUMENT;}

            int32_t *vec1_data = (int32_t *)vec1->data;
            int32_t *result_data = (int32_t *)result->data;
            int64_t val_i32 = (int64_t)val;

            for (int i = 0; i < vec1->size; i++) {
                int64_t intermediate = (int64_t)(vec1_data[i]) * val_i32;
                result_data[i] = (int32_t)(intermediate >> shift_amount);
            }
            return VECTOR_SUCCESS;
        }

        case DTYPE_FLOAT32: {
            return VECTOR_NOT_IMPLEMENTED;
        }

        default:
            return VECTOR_UNSUPPORTED_OPERATION;
    }
}   

vector_status_t scalar_mul_scalar_f32(const vector_t *vec1, const float val, vector_t *result) {
    if (vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type) {
        case DTYPE_INT8:  return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_INT16:  return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_INT32:   return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_FLOAT32: {
            float* vec1_data = (float*)(vec1->data);
            float* result_data = (float*)(result->data);
            for (int i = 0; i < vec1->size; i++){ 
                result_data[i] = vec1_data[i] * val;
            }
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_UNSUPPORTED_OPERATION;
    }
}   

vector_status_t scalar_dotp(const vector_t *vec1, const vector_t *vec2, int32_t* result) {
    if (vec1->size != vec2->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != vec2->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type){
        case DTYPE_INT8:{
            int32_t output = 0;
            int8_t *vec1_data = (int8_t*)vec1->data;
            int8_t *vec2_data = (int8_t*)vec2->data;
            for (int i = 0; i < vec1->size; i++){
                int a = (int)vec1_data[i];
                int b = (int)vec2_data[i];
                output +=  a * b;
            }
            *result = output;
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT16:{
            int32_t output = 0;
            int16_t *vec1_data = (int16_t*)vec1->data;
            int16_t *vec2_data = (int16_t*)vec2->data;
            for (int i = 0; i < vec1->size; i++){
                int a = (int)vec1_data[i];
                int b = (int)vec2_data[i];
                output +=  a * b;
            }
            *result = output;
            return VECTOR_SUCCESS;
        }
        case DTYPE_INT32:{
            int32_t output = 0;
            int32_t *vec1_data = (int32_t*)vec1->data;
            int32_t *vec2_data = (int32_t*)vec2->data;
            for (int i = 0; i < vec1->size; i++){
                int64_t a = (int64_t)vec1_data[i];
                int64_t b = (int64_t)vec2_data[i];
                output +=  (int32_t)(a * b);
            }
            *result = output;
            return VECTOR_SUCCESS;
        }
        case DTYPE_FLOAT32: return VECTOR_UNSUPPORTED_OPERATION;
        default: return VECTOR_ERROR; 
    }
}

vector_status_t scalar_dotp_f32(const vector_t *vec1, const vector_t *vec2, float* result) {
    if (vec1->size != vec2->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type) { return VECTOR_TYPE_MISMATCH;}   
    
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): {
            float output = 0;
            float* vec1_data = (float*)vec1->data;
            float* vec2_data = (float*)vec2->data;
            for (int i = 0 ; i < vec1->size; i++){
                output += vec1_data[i] * vec2_data[i];
            }
            *result = output;
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR;  
    }
}

vector_status_t scalar_abs(const vector_t *vec1,  vector_t* result) {
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}   
    
    switch (vec1->type){
        case (DTYPE_INT8):   {
            int8_t *vec1_data = (int8_t*)(vec1->data);
            int8_t *result_data = (int8_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int8_t val = vec1_data[i];
                val = val == INT8_MIN ? INT8_MAX : val;
                result_data[i] = val > 0 ? val : -val;
            } 
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT16):  {
            int16_t *vec1_data = (int16_t*)(vec1->data);
            int16_t *result_data = (int16_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int16_t val = vec1_data[i];
                val = val == INT16_MIN ? INT16_MAX : val;
                result_data[i] = val > 0 ? val : -val;
            } 
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT32):  {
            int32_t *vec1_data = (int32_t*)(vec1->data);
            int32_t *result_data = (int32_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int32_t val = vec1_data[i];
                result_data[i] = val > 0 ? val : -val;
            } 
            return VECTOR_SUCCESS;
        }
        case (DTYPE_FLOAT32): { 
            int32_t* vec1_data = (int32_t*)vec1->data;
            int32_t* result_data = (int32_t*)result->data;
            for (int i = 0 ; i < vec1->size; i++){
                int32_t val = vec1_data[i];
                val = val & INT32_MAX;
                result_data[i] = val;
            }
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR;  
    }
}

vector_status_t scalar_ceil(const vector_t *vec1, vector_t* result, const int ceiling){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8): {
            if(ceiling < INT8_MIN || ceiling > INT8_MAX) { return VECTOR_INVALID_ARGUMENT;}  
            int8_t ceil = (int8_t)ceiling; 
            int8_t *vec1_data = (int8_t*)(vec1->data);    
            int8_t *result_data = (int8_t*)(result->data);                                   
            for (int i = 0; i < vec1->size; i++){
                int8_t cur_val = vec1_data[i];
                result_data[i] = cur_val > ceil ? ceil : cur_val;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT16): { 
            if(ceiling < INT16_MIN || ceiling > INT16_MAX) { return VECTOR_INVALID_ARGUMENT;} 
            int16_t ceil = (int16_t)ceiling; 
            int16_t *vec1_data = (int16_t*)(vec1->data);    
            int16_t *result_data = (int16_t*)(result->data);                                   
            for (int i = 0; i < vec1->size; i++){
                int16_t cur_val = vec1_data[i];
                result_data[i] = cur_val > ceil ? ceil : cur_val;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT32): { 
            int32_t ceil = (int32_t)ceiling; 
            int32_t *vec1_data = (int32_t*)(vec1->data);    
            int32_t *result_data = (int32_t*)(result->data);                                   
            for (int i = 0; i < vec1->size; i++){
                int32_t cur_val = vec1_data[i];
                result_data[i] = cur_val > ceil ? ceil : cur_val;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_FLOAT32): { 
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_ceil_f32(const vector_t *vec1, vector_t* result, const float ceiling){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): {   
            float *vec1_data = (float*)(vec1->data);    
            float *result_data = (float*)(result->data);                                   
            for (int i = 0; i < vec1->size; i++){
                float cur_val = vec1_data[i];
                result_data[i] = cur_val > ceiling ? ceiling : cur_val;
            }
            return VECTOR_SUCCESS;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_floor(const vector_t *vec1, vector_t* result, const int floor_val){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8): {
            if(floor_val < INT8_MIN || floor_val > INT8_MAX) { return VECTOR_INVALID_ARGUMENT;}  
            int8_t floor = (int8_t)floor_val; 
            int8_t *vec1_data = (int8_t*)(vec1->data);    
            int8_t *result_data = (int8_t*)(result->data);                                   
            for (int i = 0; i < vec1->size; i++){
                int8_t cur_val = vec1_data[i];
                result_data[i] = cur_val < floor ? floor : cur_val;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT16): { 
            if(floor_val < INT16_MIN || floor_val > INT16_MAX) { return VECTOR_INVALID_ARGUMENT;} 
            int16_t floor = (int16_t)floor_val; 
            int16_t *vec1_data = (int16_t*)(vec1->data);    
            int16_t *result_data = (int16_t*)(result->data);                                   
            for (int i = 0; i < vec1->size; i++){
                int16_t cur_val = vec1_data[i];
                result_data[i] = cur_val < floor ? floor : cur_val;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT32): { 
            int32_t floor = (int32_t)floor_val; 
            int32_t *vec1_data = (int32_t*)(vec1->data);    
            int32_t *result_data = (int32_t*)(result->data);                                   
            for (int i = 0; i < vec1->size; i++){
                int32_t cur_val = vec1_data[i];
                result_data[i] = cur_val < floor ? floor : cur_val;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_FLOAT32): { 
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_floor_f32(const vector_t *vec1, vector_t* result, const float floor_val){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): { 
            float floor = (float)floor_val; 
            float *vec1_data = (float*)(vec1->data);    
            float *result_data = (float*)(result->data);                                   
            for (int i = 0; i < vec1->size; i++){
                float cur_val = vec1_data[i];
                result_data[i] = cur_val < floor ? floor : cur_val;
            }
            return VECTOR_SUCCESS;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_neg(const vector_t *vec1,  vector_t* result) {
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}   
    
    switch (vec1->type){
        case (DTYPE_INT8):   {
            int8_t *vec1_data = (int8_t*)(vec1->data);
            int8_t *result_data = (int8_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int8_t val = vec1_data[i];
                val = val == INT8_MIN ? -INT8_MAX : val;
                result_data[i] = -val;
            } 
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT16):  {
            int16_t *vec1_data = (int16_t*)(vec1->data);
            int16_t *result_data = (int16_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int16_t val = vec1_data[i];
                val = val == INT16_MIN ? -INT16_MAX : val;
                result_data[i] =  -val;
            } 
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT32):  {
            int32_t *vec1_data = (int32_t*)(vec1->data);
            int32_t *result_data = (int32_t*)(result->data);
            for (int i = 0; i < vec1->size; i++){
                int32_t val = vec1_data[i];
                result_data[i] = -val;
            } 
            return VECTOR_SUCCESS;
        }
        case (DTYPE_FLOAT32): { 
            float* vec1_data = (float*)vec1->data;
            float* result_data = (float*)result->data;
            for (int i = 0 ; i < vec1->size; i++){
                float val = vec1_data[i]; 
                result_data[i] = -val;
            }
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR;  
    }
}

vector_status_t scalar_mac(const vector_t *vec1, int32_t* accumulator, const int multiplier){  
    if (!accumulator) { return VECTOR_INVALID_ARGUMENT;}
    switch (vec1->type){
        case (DTYPE_INT8): {
            if (multiplier < INT8_MIN || multiplier > INT8_MAX) { return VECTOR_INVALID_ARGUMENT;} 
            int8_t* vec1_data = (int8_t*)vec1->data;
            int32_t output = 0;
            for (int i = 0; i < vec1->size; i++){
                output += vec1_data[i] * multiplier;
            }
            *accumulator += output;
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT16): {
            if (multiplier < INT16_MIN || multiplier > INT16_MAX) { return VECTOR_INVALID_ARGUMENT;} 
            int16_t* vec1_data = (int16_t*)vec1->data;
            int32_t output = 0;
            for (int i = 0; i < vec1->size; i++){
                output += vec1_data[i] * multiplier;
            }
            *accumulator += output;
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT32): {  
            int32_t* vec1_data = (int32_t*)vec1->data;
            int32_t output = 0;
            for (int i = 0; i < vec1->size; i++){
                output += vec1_data[i] * multiplier;
            }
            *accumulator += output;
            return VECTOR_SUCCESS;
        }
        case (DTYPE_FLOAT32): {
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_mac_f32(const vector_t *vec1, float* accumulator, const float multiplier){  
    if (!accumulator) { return VECTOR_INVALID_ARGUMENT;}
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): {
            float acc = *accumulator;
            float* vec1_data = (float*)(vec1->data);
            for( int i = 0; i < vec1->size; i++){
                acc += vec1_data[i] * multiplier;
            }
            *accumulator = acc;
            return VECTOR_SUCCESS;
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_zeros(vector_t *vec1){  
    switch (vec1->type){
        case (DTYPE_INT8): {  
            int8_t *vec1_data = (int8_t*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = 0;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT16): { 
            int16_t *vec1_data = (int16_t*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = 0;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT32): { 
            int32_t *vec1_data = (int32_t*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = 0;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_FLOAT32): { 
            float *vec1_data = (float*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = 0.0f;
            }
            return VECTOR_SUCCESS;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_ones(vector_t *vec1){  
    switch (vec1->type){
        case (DTYPE_INT8): {  
            int8_t *vec1_data = (int8_t*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = 1;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT16): { 
            int16_t *vec1_data = (int16_t*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = 1;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT32): { 
            int32_t *vec1_data = (int32_t*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = 1;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_FLOAT32): { 
            float *vec1_data = (float*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = 1.0f;
            }
            return VECTOR_SUCCESS;
        }
        default:        
            return VECTOR_ERROR;
    }
}
 
vector_status_t scalar_fill(vector_t *vec1, const int val){  
    switch (vec1->type){
        case (DTYPE_INT8): {  
            if (val > INT8_MAX || val < INT8_MIN) { return VECTOR_INVALID_ARGUMENT;}
            int8_t *vec1_data = (int8_t*)(vec1->data);     
            int8_t value = (int8_t)val;                                  
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = value;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT16): { 
            if (val > INT16_MAX || val < INT16_MIN) { return VECTOR_INVALID_ARGUMENT;}    
            int16_t *vec1_data = (int16_t*)(vec1->data);           
            int16_t value = (int16_t)val;                           
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = value;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_INT32): { 
            int32_t *vec1_data = (int32_t*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = val;
            }
            return VECTOR_SUCCESS;
        }
        case (DTYPE_FLOAT32): {  
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_fill_f32(vector_t *vec1, const float val){  
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): {   
            float *vec1_data = (float*)(vec1->data);                                       
            for (int i = 0; i < vec1->size; i++){ 
                vec1_data[i] = val;
            }
            return VECTOR_SUCCESS;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t scalar_copy(vector_t *vec1, vector_t *result){
    if (vec1->size != result->size) { return VECTOR_SIZE_MISMATCH;}
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}
    memcpy(result->data, vec1->data, vec1->size * sizeof_dtype(vec1->type));
    return VECTOR_SUCCESS;
}

#endif