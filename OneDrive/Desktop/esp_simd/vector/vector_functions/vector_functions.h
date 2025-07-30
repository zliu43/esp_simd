#ifndef VECTOR_FUNCTIONS_H
#define VECTOR_FUNCTIONS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {    
#endif 

extern int simd_add_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_add_scalar_i8(const int8_t *a, const int8_t *scalar_val, int8_t *result, const size_t size);  
extern int simd_sub_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_mul_i8_shift(const int8_t *a, const int8_t *b, int8_t *result, const int shift_amount, const size_t size);
extern int simd_mul_i8_to_i16(const int8_t *a, const int8_t *b, int16_t *result, const size_t size);
extern int simd_sum_i8(const int8_t *a, int32_t* result, const size_t size);
extern int simd_mul_scalar_i8(const int8_t *a, const int8_t *scalar_val, int8_t *result, const int shift_amount, const size_t size);
extern int simd_dotp_i8(const int8_t *a, const int8_t *b, int32_t* result, const size_t size);
extern int simd_abs_i8(const int8_t *a, int8_t *result, const size_t size);
extern int simd_ceil_i8(const int8_t *a, int8_t *result, const int8_t *max_val, const size_t size);
extern int simd_floor_i8(const int8_t *a, int8_t *result, const int8_t *min_val, const size_t size);
extern int simd_neg_i8(const int8_t *a, int8_t *result, const size_t size);
extern int simd_relu_i8(const int8_t *a, const int multiplier, const int shift_amount, int8_t *result, const size_t size);
extern int simd_mac_i8(const int8_t *a,  int32_t *accumulator, const int8_t *multiplier, const size_t size);
extern int simd_compare_gt_i8(const int8_t *a, const int8_t *b, uint8_t *result, const size_t size);
extern int simd_compare_lt_i8(const int8_t *a, const int8_t *b, uint8_t *result, const size_t size);
extern int simd_compare_eq_i8(const int8_t *a, const int8_t *b, uint8_t *result, const size_t size);
extern int simd_zeros_i8(int8_t *a, const size_t size);
 

// Implemented, needs test cases
extern int simd_add_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_and_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_xor_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_or_i16(const int16_t *a, const int16_t *b, int16_t *result, const size_t size);
extern int simd_abs_i16(const int16_t *a, int16_t *result, const size_t size);
extern int simd_not_i16(const int16_t *a, int16_t *result, const size_t size);

extern int simd_ones_i8(int8_t *a, const size_t size);
extern int simd_fill_i8(int8_t *a, const int8_t *val, const size_t size);
extern int simd_xor_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_and_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_or_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size); 
extern int simd_max_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_min_i8(const int8_t *a, const int8_t *b, int8_t *result, const size_t size);
extern int simd_not_i8(const int8_t *a, int8_t *result, const size_t size); 

//TODO - NOT YET IMPLEMENTED 
extern int simd_bound(const int8_t *a, const int8_t *max_val, const int8_t *min_val, int8_t *result, const size_t size);


#ifdef __cplusplus
}
#endif

#endif // VECTOR_FUNCTIONS_H