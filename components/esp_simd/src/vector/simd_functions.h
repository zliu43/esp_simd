#ifndef SIMD_FUNCTIONS_H
#define SIMD_FUNCTIONS_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {    
#endif  


//int8_t
extern int simd_add_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_add_scalar_i8(const int8_t *a, const int8_t *scalar_val, int8_t *result, const size_t size);  
extern int simd_sub_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_mul_shift_i8(const int8_t *a, const int8_t *b, int8_t *result, const int shift_amount, const size_t size);
extern int simd_mul_i8_to_i16(const int8_t *a, const int8_t *b, int16_t *result, const size_t size);
extern int simd_sum_i8(const int8_t *a, int32_t* result, const size_t size);
extern int simd_mul_scalar_i8(const int8_t *a, const int8_t *scalar_val, int8_t *result, const unsigned int shift_amount, const size_t size);
extern int simd_dotp_i8(const int8_t *a, const int8_t *b, int32_t* result, const size_t size);
extern int simd_abs_i8(const int8_t *a, int8_t *result, const size_t size);
extern int simd_ceil_i8(const int8_t *a, int8_t *result, const int8_t *max_val, const size_t size);
extern int simd_floor_i8(const int8_t *a, int8_t *result, const int8_t *min_val, const size_t size);
extern int simd_neg_i8(const int8_t *a, int8_t *result, const size_t size);
extern int simd_relu_i8(const int8_t *a, const int multiplier, const unsigned int shift_amount, int8_t *result, const size_t size);
extern int simd_mac_i8(const int8_t *a,  int32_t *accumulator, const int8_t *multiplier, const size_t size);
extern int simd_compare_gt_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_compare_lt_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_compare_eq_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_zeros_i8(int8_t *a, const size_t size);
extern int simd_ones_i8(int8_t *a, const size_t size);
extern int simd_fill_i8(int8_t *a, const int8_t *val, const size_t size);
extern int simd_xor_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_and_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_or_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size); 
extern int simd_max_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_min_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_not_i8(const int8_t *a, int8_t *result, const size_t size);  
extern int simd_copy_i8(const int8_t *a, int8_t *result, const size_t size); 
 

// int16_t
extern int simd_abs_i16(const int16_t *a, int16_t *result, const size_t size);
extern int simd_add_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size); 
extern int simd_add_scalar_i16(const int16_t *a, const int16_t *scalar_val, int16_t *result, const size_t size);
extern int simd_and_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_ceil_i16(const int16_t *a, int16_t *result, const int16_t *max_val, const size_t size); 
extern int simd_compare_eq_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_compare_gt_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_compare_lt_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_dotp_i16(const int16_t *a, const int16_t *b, int32_t* result, const size_t size);
extern int simd_fill_i16(int16_t *a, const int16_t *val, const size_t size);
extern int simd_floor_i16(const int16_t *a, int16_t *result, const int16_t *min_val, const size_t size);
extern int simd_max_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_min_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_mac_i16(const int16_t *a,  int32_t *accumulator, const int16_t *multiplier, const size_t size);
extern int simd_mul_i16_to_i32(const int16_t *a, const int16_t *b, int32_t *result, const size_t size);
extern int simd_mul_scalar_i16(const int16_t *a, const int16_t *scalar_val, int16_t *result, const unsigned int shift_amount, const size_t size);
extern int simd_mul_shift_i16(const int16_t *a, const int16_t *b, int16_t *result, const unsigned int shift_amount, const size_t size);
extern int simd_neg_i16(const int16_t *a, int16_t *result, const size_t size);
extern int simd_not_i16(const int16_t *a, int16_t *result, const size_t size);
extern int simd_ones_i16(int16_t *a, const size_t size);
extern int simd_or_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_relu_i16(const int16_t *a, const int multiplier, const unsigned int shift_amount, int16_t *result, const size_t size);
extern int simd_sub_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_sum_i16(const int16_t *a, int32_t* result, const size_t size);
extern int simd_xor_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_zeros_i16(int16_t *a, const size_t size);
extern int simd_copy_i16(const int16_t *a, int16_t *result, const size_t size); 



// int32_t
extern int simd_abs_i32(const int32_t *a, int32_t *result, const size_t size);
extern int simd_add_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size);
extern int simd_add_scalar_i32(const int32_t *a, const int32_t *scalar_val, int32_t *result, const size_t size);
extern int simd_and_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size);
extern int simd_ceil_i32(const int32_t *a, int32_t *result, const int32_t *max_val, const size_t size);
extern int simd_compare_eq_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size);
extern int simd_compare_gt_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size);
extern int simd_compare_lt_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size); 
extern int simd_dotp_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size); 
extern int simd_fill_i32(int32_t *a, const int32_t *val, const size_t size);
extern int simd_floor_i32(const int32_t *a, int32_t *result, const int32_t *min_val, const size_t size);
extern int simd_max_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size);
extern int simd_min_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size); 
extern int simd_mac_i32(const int32_t *a, int32_t *accumulator, const int32_t *multiplier, const size_t size); 
extern int simd_mul_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size);
extern int simd_mul_scalar_i32(const int32_t *a, const int32_t *scalar_val, int32_t *result, const unsigned int shift_amount, const size_t size);
extern int simd_mul_shift_i32(const int32_t *a, const int32_t *b, int32_t *result, const unsigned int shift_amount, const size_t size); 
extern int simd_neg_i32(const int32_t *a, int32_t *result, const size_t size);
extern int simd_not_i32(const int32_t *a, int32_t *result, const size_t size);
extern int simd_ones_i32(int32_t *a, const size_t size);
extern int simd_or_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size); 
extern int simd_relu_i32(const int32_t *a, const int multiplier, const unsigned int shift_amount, int32_t *result, const size_t size); 
extern int simd_sub_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size); 
extern int simd_sum_i32(const int32_t *a, int32_t *result, const size_t size); 
extern int simd_xor_i32(const int32_t *a, const int32_t *b, int32_t *result, const size_t size);
extern int simd_zeros_i32(int32_t *a, const size_t size);
extern int simd_copy_i32(const int32_t *a, int32_t *result, const size_t size); 

// float32
extern int simd_abs_f32(const float* a, float* result, const size_t size);
extern int simd_add_f32(const float* a, const float* b,float* result, const size_t size);
extern int simd_sub_f32(const float* a, const float* b,float* result, const size_t size);
extern int simd_add_scalar_f32(const float *a, const float *scalar_val, float *result, const size_t size);
extern int simd_mul_shift_f32(const float *a, const float *b, float *result, const unsigned int shift_amount, const size_t size);  
extern int simd_sum_f32(const float *a, float * result, const size_t size);
extern int simd_mul_scalar_f32(const float* a, const float* scalar_val, float* result, const size_t size);
extern int simd_dotp_f32(const float *a, const float *b, float *result, const size_t size);
extern int simd_ceil_f32(const float *a, float *result, const float *max_val, const size_t size);
extern int simd_floor_f32(const float *a, float *result, const float *min_val, const size_t size);
extern int simd_neg_f32(const float *a, float *result, const size_t size);
extern int simd_mac_f32(const float *a, float *accumulator, const float *multiplier, const size_t size); 

//TODO - NOT YET IMPLEMENTED 
//extern int simd_bound(const int8_t *a, const int8_t *max_val, const int8_t *min_val, int8_t *result, const size_t size);


#ifdef __cplusplus
}
#endif

#endif // VECTOR_FUNCTIONS_H