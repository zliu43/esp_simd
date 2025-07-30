#ifndef SCALAR_FUNCTIONS_H
#define SCALAR_FUNCTIONS_H

#include "vector.h"
#include "stdlib.h"


void scalar_add_i8_scalar(const vector_t *vec1, const int scalar, vector_t *result);
void scalar_add_i8(const vector_t *vec1,const  vector_t *vec2, vector_t *result);
void scalar_sub_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result);
void scalar_abs_i8(const vector_t *vec1, vector_t *result);
void scalar_ceil_i8(const vector_t *vec1, vector_t *result, const int ceiling);
void scalar_floor_i8(const vector_t *vec1, vector_t *result,  const int floor);
void scalar_compare_eq_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result);
void scalar_compare_gt_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result);
void scalar_compare_lt_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result);
void scalar_dotp_i8(const vector_t *vec1, const vector_t *vec2, int32_t *result);
void scalar_neg_i8(const vector_t *vec1, vector_t *result);
void scalar_abs_i8(const vector_t *vec1, vector_t *result);
void scalar_mul_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result);
void scalar_mul_shift_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result, const int shift_amount);
void scalar_mac_i8(const vector_t *vec1, int32_t* accumulator, const int multiplier);
void scalar_zero_i8(vector_t *vec);
void scalar_relu_i8(const vector_t *vec1, vector_t *result, const int multiplier, const int shift_amount);
void scalar_mul_scalar_shift_i8(const vector_t *vec1, const int val, vector_t *result, const int shift_amount);
void scalar_mul_scalar_i8(const vector_t *vec1, const int val, vector_t *result);
void scalar_add_scalar_i8(const vector_t *vec1, const int scalar, vector_t *result);
void scalar_sum_i8(const vector_t *vec1, int32_t* result);

#endif