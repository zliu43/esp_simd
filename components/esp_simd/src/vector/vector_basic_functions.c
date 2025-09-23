#include "vector_basic_functions.h"
#include "simd_functions.h"

vector_status_t vec_add(const vector_t *vec1, const vector_t *vec2, vector_t *result) { 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type) {
        case DTYPE_INT8: {
            return simd_add_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size); 
        }
        case DTYPE_INT16: { 
            return simd_add_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case DTYPE_INT32: {
            return simd_add_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case DTYPE_FLOAT32: {
            return simd_add_f32((float*)(vec1->data), (float*)(vec2->data), (float*)(result->data), vec1->size); 
        }
        default:
            return VECTOR_ERROR; 
    }   
}
 
vector_status_t vec_sub(const vector_t *vec1, const vector_t *vec2, vector_t *result) {  
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;} 
    switch (vec1->type) {
        case DTYPE_INT8: {
            return simd_sub_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), vec1->size);                                             
        }
        case DTYPE_INT16: {
            return simd_sub_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), vec1->size); 
        }
        case DTYPE_INT32: {
            return simd_sub_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), vec1->size); 
        }
        case DTYPE_FLOAT32: { 
            return simd_sub_f32((float*)(vec1->data), (float*)(vec2->data), (float*)(result->data), vec1->size);  
        }
        default:
            return VECTOR_ERROR;                                  
    } 
}

vector_status_t vec_add_scalar(const vector_t *vec1, const int value, vector_t *result) { 
    if (vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type)
    {
        case DTYPE_INT8:{
            if(value < INT8_MIN || value > INT8_MAX) {return VECTOR_INVALID_ARGUMENT;}     
            int8_t val = (int8_t)value;                                                     
            return simd_add_scalar_i8((int8_t*)(vec1->data), &val, (int8_t*)(result->data), vec1->size);  
        }                                                  
        case DTYPE_INT16:{
            if(value < INT16_MIN  || value > INT16_MAX) {return VECTOR_INVALID_ARGUMENT;}    
            int16_t val = (int16_t)value;                                                
            return simd_add_scalar_i16((int16_t*)(vec1->data), &val, (int16_t*)(result->data), vec1->size);  
        }
        case DTYPE_INT32:{  
            int32_t val = (int32_t)value;             
            return simd_add_scalar_i32((int32_t*)(vec1->data), &val, (int32_t*)(result->data), vec1->size);  
        }
        case DTYPE_FLOAT32:{
            return VECTOR_UNSUPPORTED_OPERATION; // Please use vec_add_scalar_f32()
        }
        default:
            return VECTOR_ERROR;
    }  
}

vector_status_t vec_add_scalar_f32(const vector_t *vec1, const float value, vector_t *result) { 
    if (vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}
    switch (vec1->type)
    {
        case DTYPE_INT8: return VECTOR_UNSUPPORTED_OPERATION;                                 
        case DTYPE_INT16: return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_INT32: return VECTOR_UNSUPPORTED_OPERATION;
        case DTYPE_FLOAT32:{
            return simd_add_scalar_f32((float*)(vec1->data), &value, (float*)(result->data), vec1->size);
        }
        default:
            return VECTOR_ERROR;
    }  
}

vector_status_t vec_mul(const vector_t *vec1, const vector_t *vec2, vector_t *result, const unsigned int shift_amount) { 
    if (vec1->size != vec2->size || vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type || vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type) {
        case DTYPE_INT8: { 
            return simd_mul_shift_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), (int8_t*)(result->data), shift_amount, vec1->size);
        }
        case DTYPE_INT16: { 
            return simd_mul_shift_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), (int16_t*)(result->data), shift_amount, vec1->size); 
        }
        case DTYPE_INT32: { 
            return simd_mul_shift_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), (int32_t*)(result->data), shift_amount, vec1->size);
        }
        case DTYPE_FLOAT32: {
            return simd_mul_shift_f32((float*)(vec1->data), (float*)(vec2->data), (float*)(result->data), shift_amount, vec1->size);  
        }
        default:
            return VECTOR_ERROR; 
    }  
}

vector_status_t vec_sum(const vector_t *vec1, int32_t* result){ 
    switch (vec1->type){
        case(DTYPE_INT8): { 
            return simd_sum_i8((int8_t*)(vec1->data), result, vec1->size); 
        }
        case(DTYPE_INT16): {
            return simd_sum_i16((int16_t*)(vec1->data), result, vec1->size); 
        }
        case(DTYPE_INT32): {
            return simd_sum_i32((int32_t*)(vec1->data), result, vec1->size); 
        }
        case(DTYPE_FLOAT32): {
            return VECTOR_UNSUPPORTED_OPERATION; // Please use vec_sum_f32
        }
        default:
            return VECTOR_ERROR;
    } 
}

vector_status_t vec_sum_f32(const vector_t *vec1, float* result){ 
    switch (vec1->type){
        case(DTYPE_INT8): return VECTOR_UNSUPPORTED_OPERATION;  
        case(DTYPE_INT16): return VECTOR_UNSUPPORTED_OPERATION;   
        case(DTYPE_INT32): return VECTOR_UNSUPPORTED_OPERATION;  
        case(DTYPE_FLOAT32):  {
            return simd_sum_f32((float*)vec1->data, result, vec1->size); // Please use vec_sum
        }
        default:
            return VECTOR_ERROR;
    } 
}

vector_status_t vec_mul_scalar(const vector_t *vec1, const int value, vector_t *result, const unsigned int shift_amount) {  
    if (vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type) {
        case DTYPE_INT8: {
            if(value < INT8_MIN || value > INT8_MAX) {return VECTOR_INVALID_ARGUMENT;}          
            int8_t val = (int8_t)value; 
            return simd_mul_scalar_i8(vec1->data, &val, result->data, shift_amount, vec1->size); 
        }
        case DTYPE_INT16: {
            if(value < INT16_MIN || value > INT16_MAX) {return VECTOR_INVALID_ARGUMENT;}   
            int16_t val = (int16_t)value;   
            return simd_mul_scalar_i16(vec1->data, &val, result->data, shift_amount, vec1->size); 
        }
        case DTYPE_INT32: {  
            int32_t val = (int32_t)value;   
            return simd_mul_scalar_i32((int32_t*)(vec1->data), &val, (int32_t*)(result->data), shift_amount, vec1->size); 
        }
        case DTYPE_FLOAT32: {
            return VECTOR_UNSUPPORTED_OPERATION;  
        }
        default:
            return VECTOR_ERROR;                                                 
    }  
}

vector_status_t vec_mul_scalar_f32(const vector_t *vec1, const float value, vector_t *result) {  
    if (vec1->size != result->size){ return VECTOR_SIZE_MISMATCH;} 
    if (vec1->type != result->type){ return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type) {
        case DTYPE_INT8: return VECTOR_UNSUPPORTED_OPERATION;  
        case DTYPE_INT16: return VECTOR_UNSUPPORTED_OPERATION;   
        case DTYPE_INT32: return VECTOR_UNSUPPORTED_OPERATION;  
        case DTYPE_FLOAT32: {
            return simd_mul_scalar_f32((float*)(vec1->data), &value, (float*)(result->data), vec1->size); 
        }
        default:
            return VECTOR_ERROR;                                                 
    }  
}

vector_status_t vec_dotp(const vector_t *vec1, const vector_t *vec2, int32_t *result){   
    if (vec1->size != vec2->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type) { return VECTOR_TYPE_MISMATCH;}   
    switch (vec1->type){
        case (DTYPE_INT8): {
            return simd_dotp_i8((int8_t*)(vec1->data), (int8_t*)(vec2->data), result,vec1->size); 
        }
        case (DTYPE_INT16): {
            return simd_dotp_i16((int16_t*)(vec1->data), (int16_t*)(vec2->data), result,vec1->size); 
        }
        case (DTYPE_INT32): {
            return simd_dotp_i32((int32_t*)(vec1->data), (int32_t*)(vec2->data), result,vec1->size); 
        }
        case (DTYPE_FLOAT32): { 
            return VECTOR_UNSUPPORTED_OPERATION; // Please use vec_dotp_f32
        }
        default:
            return VECTOR_ERROR; 
    }
}

vector_status_t vec_dotp_f32(const vector_t *vec1, const vector_t *vec2, float *result){   
    if (vec1->size != vec2->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != vec2->type) { return VECTOR_TYPE_MISMATCH;}   
    
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): {
            return simd_dotp_f32((float*)(vec1->data), (float*)(vec2->data), result,vec1->size); 
        }
        default:
            return VECTOR_ERROR;  
    }
}

vector_status_t vec_abs(const vector_t *vec1, vector_t* result){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}   

    switch (vec1->type){
        case (DTYPE_INT8): {
            return simd_abs_i8((int8_t*)(vec1->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): {
            return simd_abs_i16((int16_t*)(vec1->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_abs_i32((int32_t*)(vec1->data), (int32_t*)(result->data), vec1->size);
        };
        case (DTYPE_FLOAT32): { 
            return simd_abs_f32((float*)(vec1->data), (float*)(result->data), vec1->size);
        };
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_ceil(const vector_t *vec1, vector_t* result, const int ceiling){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8): {
            if(ceiling < INT8_MIN || ceiling > INT8_MAX) { return VECTOR_INVALID_ARGUMENT;}                  // Ensure ceiling is within int8_t range
            int8_t ceil = (int8_t)ceiling;                                  // Cast int to int8_t pointer
            return simd_ceil_i8((int8_t*)(vec1->data), (int8_t*)(result->data), &ceil, vec1->size); 
        }
        case (DTYPE_INT16): { 
            if(ceiling < INT16_MIN || ceiling > INT16_MAX) { return VECTOR_INVALID_ARGUMENT;}                 // Ensure ceiling is within int16_t range
            int16_t ceil = (int16_t)ceiling;                                // Cast int to int16_t pointer
            return simd_ceil_i16((int16_t*)(vec1->data), (int16_t*)(result->data), &ceil, vec1->size); 
        }
        case (DTYPE_INT32): { 
            int32_t ceil = (int32_t)ceiling;                                // Cast int to int16_t pointer
            return simd_ceil_i32((int32_t*)(vec1->data), (int32_t*)(result->data), &ceil, vec1->size); 
        }
        case (DTYPE_FLOAT32): { 
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t vec_ceil_f32(const vector_t *vec1, vector_t* result, const float ceiling){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): { 
            return simd_ceil_f32((float*)(vec1->data), (float*)(result->data), &ceiling, vec1->size);
        }
        default:        
            return VECTOR_NOT_IMPLEMENTED;
    }
}

vector_status_t vec_floor(const vector_t *vec1, vector_t* result, const int floor){  
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8): {
            if (floor < INT8_MIN || floor > INT8_MAX) { return VECTOR_INVALID_ARGUMENT;}
            int8_t min_val = (int8_t)floor;
            return simd_floor_i8((int8_t*)(vec1->data), (int8_t*)(result->data), &min_val, vec1->size); 
        }
        case (DTYPE_INT16): {
            if (floor < INT16_MIN || floor > INT16_MAX) { return VECTOR_INVALID_ARGUMENT;}
            int16_t min_val = (int16_t)floor;
            return simd_floor_i16((int16_t*)(vec1->data), (int16_t*)(result->data), &min_val, vec1->size);  
        }
        case (DTYPE_INT32): { 
            int32_t min_val = (int32_t)floor;
            return simd_floor_i32((int32_t*)(vec1->data), (int32_t*)(result->data), &min_val, vec1->size); 
        }
        case (DTYPE_FLOAT32): { 
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t vec_floor_f32(const vector_t *vec1, vector_t* result, const float ceiling){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): { 
            return simd_floor_f32((float*)(vec1->data), (float*)(result->data), &ceiling, vec1->size);
        }
        default:        
            return VECTOR_ERROR;
    }
}

vector_status_t vec_neg(const vector_t *vec1, vector_t* result){  
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8): {
            return simd_neg_i8((int8_t*)(vec1->data), (int8_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT16): {
            return simd_neg_i16((int16_t*)(vec1->data), (int16_t*)(result->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_neg_i32((int32_t*)(vec1->data), (int32_t*)(result->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): { 
            return simd_neg_f32((float*)(vec1->data), (float*)(result->data), vec1->size); 
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_mac(const vector_t *vec1, int32_t* accumulator, const int multiplier){  
    if (!accumulator) { return VECTOR_INVALID_ARGUMENT;}
    switch (vec1->type){
        case (DTYPE_INT8): {
            if (multiplier < INT8_MIN || multiplier > INT8_MAX) { return VECTOR_INVALID_ARGUMENT;}
            int8_t multiplier_i8 = (int8_t)multiplier;
            return simd_mac_i8((int8_t*)(vec1->data), accumulator, &multiplier_i8, vec1->size); 
        }
        case (DTYPE_INT16): {
            if (multiplier < INT16_MIN || multiplier > INT16_MAX) { return VECTOR_INVALID_ARGUMENT;}
            int16_t multiplier_i16 = (int16_t)multiplier;
            return simd_mac_i16((int16_t*)(vec1->data), accumulator, &multiplier_i16, vec1->size); 
        }
        case (DTYPE_INT32): { 
            int32_t multiplier_i32 = (int32_t)multiplier;
            return simd_mac_i32((int32_t*)(vec1->data), accumulator, &multiplier_i32, vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return VECTOR_UNSUPPORTED_OPERATION;
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_mac_f32(const vector_t *vec1, float* accumulator, const float multiplier){  
    if (!accumulator) { return VECTOR_INVALID_ARGUMENT;}
    switch (vec1->type){
        case (DTYPE_INT8):   return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):  return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): {
            return simd_mac_f32((float*)(vec1->data), accumulator, &multiplier, vec1->size);  
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_zeros(vector_t *vec1){ 
    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_zeros_i8((int8_t*)(vec1->data), vec1->size); 
        }
        case (DTYPE_INT16): { 
            return simd_zeros_i16((int16_t*)(vec1->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_zeros_i32((int32_t*)(vec1->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return simd_zeros_i32((int32_t*)(vec1->data), vec1->size); 
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_ones(vector_t *vec1){ 
    switch (vec1->type){
        case (DTYPE_INT8): { 
            return simd_ones_i8((int8_t*)(vec1->data), vec1->size);
        }
        case (DTYPE_INT16): { 
            return simd_ones_i16((int16_t*)(vec1->data), vec1->size); 
        }
        case (DTYPE_INT32): { 
            return simd_ones_i32((int32_t*)(vec1->data), vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            int32_t mask = 0x3F800000;
            return simd_fill_i32((int32_t*)(vec1->data), &mask, vec1->size);
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_fill(vector_t *vec1, const int val){ 
    switch (vec1->type){
        case (DTYPE_INT8): { 
            if (val < INT8_MIN || val > INT8_MAX) { return VECTOR_INVALID_ARGUMENT;}
            int8_t val_i8 = (int8_t)val;  
            return simd_fill_i8((int8_t*)(vec1->data), &val_i8, vec1->size); 
        }
        case (DTYPE_INT16): { 
            if (val < INT16_MIN || val > INT16_MAX) { return VECTOR_INVALID_ARGUMENT;}
            int16_t val_i16 = (int16_t)val;  
            return simd_fill_i16((int16_t*)(vec1->data), &val_i16, vec1->size); 
        }
        case (DTYPE_INT32): {  
            int32_t val_i32 = (int32_t)val;  
            return simd_fill_i32((int32_t*)(vec1->data), &val_i32, vec1->size); 
        }
        case (DTYPE_FLOAT32): {
            return VECTOR_NOT_IMPLEMENTED; // Please use vec_fill_f32() instead                                         
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_fill_f32(vector_t *vec1, const float val){ 
    switch (vec1->type){
        case (DTYPE_INT8):      return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT16):     return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_INT32):     return VECTOR_UNSUPPORTED_OPERATION;
        case (DTYPE_FLOAT32): { 
            float* val_ptr = &val;
            int32_t* ptr = (int32_t*)val_ptr;
            return simd_fill_i32((int32_t*)(vec1->data), ptr, vec1->size);                                        
        }
        default:
            return VECTOR_ERROR;
    }
}

vector_status_t vec_copy(vector_t *vec1, vector_t *result){ 
    if (vec1->size != result->size ) { return VECTOR_SIZE_MISMATCH;}  
    if (vec1->type != result->type) { return VECTOR_TYPE_MISMATCH;}  
    switch (vec1->type){
        case (DTYPE_INT8): {   
            return simd_copy_i8((int8_t*)(vec1->data), (int8_t*)(result->data), vec1->size);
        }
        case (DTYPE_INT16): {  
            return simd_copy_i16((int16_t*)(vec1->data), (int16_t*)(result->data), vec1->size);
        }
        case (DTYPE_INT32): {   
            return simd_copy_i32((int32_t*)(vec1->data), (int32_t*)(result->data), vec1->size);
        }
        case (DTYPE_FLOAT32): {
            return simd_copy_i32((int32_t*)(vec1->data), (int32_t*)(result->data), vec1->size);
        }
        default:
            return VECTOR_ERROR;
    }
}
