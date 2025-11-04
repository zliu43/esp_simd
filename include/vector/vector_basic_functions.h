#ifndef VECTOR_BASIC_FUNCTIONS_H
#define VECTOR_BASIC_FUNCTIONS_H

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif
 
/**
 * @brief Element-wise addition of two vectors.
 *
 * Performs @p result[i] = @p vec1[i] + @p vec2[i] for @p size elements.
 * On ESP32-S3 builds, INT8/INT16/INT32 paths are SIMD-accelerated; 
 * FLOAT32 operations are scalar, but benefit from loop unrolling and 128-bit databus.
 *
 * @param vec1    Left operand (same dtype/size as @p vec2 and @p result).
 * @param vec2    Right operand.
 * @param result  Output vector; may alias @p vec1 or @p vec2 for in-place operation.
 *
 * @retval VECTOR_SUCCESS          Operation completed. 
 * @retval VECTOR_SIZE_MISMATCH    Sizes differ.
 * @retval VECTOR_TYPE_MISMATCH    Dtypes differ or are unsupported. 
 *
 * @note INT8/INT16/INT32 results saturate on overflow.  
 * @note FLOAT32 uses scalar FPU arithmetic; wider loads/stores may still improve bandwidth.
 * @note It is recommended to call ::vector_ok() on all input/output vectors before performing arithmetic operations.
 * @pre All vectors must have identical size and dtype. Data must be 16-byte aligned.
 */
vector_status_t vec_add(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Element-wise subtraction of two vectors.
 *
 * Performs @p result[i] = @p vec1[i] - @p vec2[i] for @p size elements.
 * On ESP32-S3 builds, INT8/INT16/INT32 paths are SIMD-accelerated; 
 * FLOAT32 operations are scalar, but benefit from loop unrolling and 128-bit databus.
 *
 * @param vec1    Left operand (same dtype/size as @p vec2 and @p result).
 * @param vec2    Right operand.
 * @param result  Output vector; may alias @p vec1 or @p vec2 for in-place operation.
 *
 * @retval VECTOR_SUCCESS          Operation completed. 
 * @retval VECTOR_SIZE_MISMATCH    Sizes differ.
 * @retval VECTOR_TYPE_MISMATCH    Dtypes differ or are unsupported. 
 *
 * @note INT8/INT16/32 results saturate on overflow/underflow.
 * @note FLOAT32 uses scalar FPU arithmetic; wider loads/stores may still improve bandwidth.
 * @note It is recommended to call ::vector_ok() on all input/output vectors before performing arithmetic operations.
 * @pre All vectors must have identical size and dtype. Data must be 16-byte aligned.
 */
vector_status_t vec_sub(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Element-wise multiply with post-shift (fixed-point style).
 *
 * For integer types (INT8/INT16/INT32), performs a component-wise multiplication
 * of two vectors, followed by a logical right shift. The shift discards lower-order
 * bits and keeps the upper N bits, where N corresponds to the operand width (8, 16, or 32),
 * effectively applying fixed-point scaling by 2^(-@p shift_amount).
 *
 * For floating-point (FLOAT32), the shift value is ignored and the result is simply
 * the element-wise product.
 *
 * - For INT8/INT16/INT32: @p result[i] = (@p vec1[i] * @p vec2[i]) >> @p shift_amount.
 * - For FLOAT32: @p result[i] = @p vec1[i] * @p vec2[i].
 *
 * @param vec1          Left operand (same dtype/size as @p vec2 and @p result).
 * @param vec2          Right operand; may alias @p vec1
 * @param result        Output vector; may alias @p vec1 or @p vec2.
 * @param shift_amount  Number of bits to shift after multiply (ignored for FLOAT32).
 *
 * @retval VECTOR_SUCCESS          Operation completed.
 * @retval VECTOR_SIZE_MISMATCH    Sizes differ.
 * @retval VECTOR_TYPE_MISMATCH    Dtypes differ or are unsupported.
 *
 * @note It is recommended to call ::vector_ok() on all vectors before performing arithmetic operations.
 * @pre All vectors must have identical size and dtype. Data must be 16-byte aligned.
 */
vector_status_t vec_mul(const vector_t *vec1, const vector_t *vec2, vector_t *result, const unsigned int shift_amount);

/**
 * @brief Add a scalar to each element (integer types only).
 *
 * Computes @p result[i] = @p vec1[i] + @p value.
 * On ESP32-S3 builds, INT8/INT16/INT32 paths are SIMD-accelerated.
 * FLOAT32 is not supported by this function-use ::vec_add_scalar_f32
 * for single-precision floating-point vectors.
 *
 * @param vec1    Input vector.
 * @param value   Scalar to add. Must be within the representable range
 *                of the vector element type.
 * @param result  Output vector; may alias @p vec1.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_NULL
 * @retval VECTOR_UNSUPPORTED_OPERATION  Unsupported dtype (e.g., FLOAT32).
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH
 * @retval VECTOR_UNALIGNED_DATA
 *
 * @note INT8/INT16/INT32 results saturate on overflow;
 * @note It is recommended to call ::vector_ok() before arithmetic operations.
 * @pre Same dtype/size for @p vec1 and @p result; 16-byte alignment required.
 */
vector_status_t vec_add_scalar(const vector_t *vec1, const int value, vector_t *result);

/**
 * @brief Add a scalar to each element (float only).
 *
 * Computes @p result[i] = @p vec1[i] + @p value. 
 * Integer types are not supported by this function-use ::vec_add_scalar 
 *
 * @param vec1    Input vector.
 * @param value   Scalar to add.  
 * @param result  Output vector; may alias @p vec1.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_NULL
 * @retval VECTOR_UNSUPPORTED_OPERATION  Unsupported dtype  
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH
 * @retval VECTOR_UNALIGNED_DATA
 * 
 * @note It is recommended to call ::vector_ok() before arithmetic operations.
 * @pre Same dtype/size for @p vec1 and @p result; 16-byte alignment required.
 */
vector_status_t vec_add_scalar_f32(const vector_t *vec1, const float value, vector_t *result);

/**
 * @brief Sum-reduce all elements into a 32-bit accumulator. Behavior is undefined for over/underflow.
 * FLOAT32 is not supported by this function-use ::vec_sum_f32()
 *
 * Computes the sum S = Σ @p vec1[i] and writes it to @p result.
 *
 * @param vec1    Input vector.
 * @param result  Pointer to 32-bit accumulator to receive the sum.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_TYPE_MISMATCH 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when @p vec1 has type DTYPE_FLOAT32;
 *                                       use ::vec_sum_f32() instead.
 *
 * @note For INT8/INT16/INT32, SIMD acceleration is used; accumulation is in 32 bits.
 * @note Large sums may overflow 32-bit accumulator.
 * @note It is recommended to call ::vector_ok() before reductions.
 */
vector_status_t vec_sum(const vector_t *vec1, int32_t *result);

/**
 * @brief Sum-reduce all elements into a 32-bit accumulator. For use with 32 bit float type.
 *
 * Computes the sum S = Σ @p vec1[i] and writes it to @p result.
 *
 * @param vec1    Input vector.
 * @param result  Pointer to 32-bit accumulator to receive the sum.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_TYPE_MISMATCH 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when @p vec1 has type integer type;
 *                                       use ::vec_sum () instead.
 * 
 * @note Does not use SIMD math, but benefits significantly from 128 bit databus and loop unrolling.
 * @note It is recommended to call ::vector_ok() before reductions.
 */
vector_status_t vec_sum_f32(const vector_t *vec1, float *result);

/**
 * @brief Multiply by a scalar with post-shift (fixed-point style). 
 * FLOAT32 is not supported by this function-use ::vec_mul_scalar_f32()
 *
 * Computes @p result[i] = (@p vec1[i] * @p value) >> @p shift_amount.
 * SHIFT is a logical bitwise right shift.
 * INT8/INT16 are SIMD-accelerated; INT32 is scalar.
 *
 * @param vec1          Input vector.
 * @param value         Integer scalar must be within valid range of DTYPE.
 * @param result        Output vector; may alias @p vec1.
 * @param shift_amount  Bits to shift after multiplication.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when @p vec1 has type DTYPE_FLOAT32;
 *                                       use ::vec_mul_scalar_f32() instead.
 * @retval VECTOR_INVALID_ARGUMENT  Shift value invalid for given dtype
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 * 
 * @note It is recommended to call ::vector_ok() before arithmetic operations.
 * @pre Same dtype/size for @p vec1 and @p result; 16-byte alignment required.
 */
vector_status_t vec_mul_scalar(const vector_t *vec1, const int value, vector_t *result, const unsigned int shift_amount);

/**
 * @brief Multiply by a scalar.
 *
 * Computes @p result[i] = (@p vec1[i] * @p value).  
 *
 * @param vec1          Input vector.
 * @param value         Integer scalar must be within valid range of DTYPE.
 * @param result        Output vector; may alias @p vec1.
 * @param shift_amount  Bits to shift after multiplication.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Unsupported dtype. Please use vec_mul_scalar() for integer types.
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 * 
 * @note It is recommended to call ::vector_ok() before arithmetic operations.
 * @pre Same dtype/size for @p vec1 and @p result; 16-byte alignment required.
 */
vector_status_t vec_mul_scalar_f32(const vector_t *vec1, const float value, vector_t *result);

/**
 * @brief Dot product into a 32-bit accumulator.
 *
 * Computes *result = Σ (@p vec1[i] * @p vec2[i]).
 * INT8/INT16 are SIMD-accelerated; INT32 is scalar.
 *
 * @param vec1    Left operand.
 * @param vec2    Right operand.
 * @param result  Pointer to 32-bit accumulator to receive the dot product.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when dtype is unsupported.
 *                                       For DTYPE_FLOAT32, use ::vector_dotp_f32() instead.
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note Accumulation is in 32 bits; behavior undefined on overflow.
 * @note Recommend ::vector_ok() on all vectors before the operation.
 */
vector_status_t vec_dotp(const vector_t *vec1, const vector_t *vec2, int32_t *result);

/**
 * @brief Dot product of two vectors into a 32-bit float accumulator.
 * For use with float type.
 *
 * Computes *result = Σ (@p vec1[i] * @p vec2[i]). 
 *
 * @param vec1    Left operand.
 * @param vec2    Right operand.
 * @param result  Pointer to 32-bit accumulator to receive the dot product.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when dtype is unsupported.
 *                                       For integer types, use ::vector_dotp() instead.
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 * 
 * @note Recommend ::vector_ok() on all vectors before the operation.
 */
vector_status_t vec_dotp_f32(const vector_t *vec1, const vector_t *vec2, float *result);

/**
 * @brief Saturating absolute value (handles most-negative value).
 *
 * Computes @p result[i] = |@p vec1[i]| with fixups:
 * - INT8: -128 → 127
 * - INT16: INT16_MIN → INT16_MAX
 *
 * @param vec1    Input vector.
 * @param result  Output vector; may alias @p vec1.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_NULL
 * @retval VECTOR_UNSUPPORTED_OPERATION  Unsupported dtype.
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH
 * @retval VECTOR_UNALIGNED_DATA
 *
 * @note INT32 uses standard abs(), no saturation.
 * @note Call ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_abs(const vector_t *vec1, vector_t* result);

/**
 * @brief Element-wise ceiling against an integer threshold.
 *
 * Computes @p result[i] = min(@p vec1[i], @p ceiling).
 *
 * @param vec1     Input vector (integer dtypes).
 * @param result   Output vector; may alias @p vec1.
 * @param ceiling  Integer upper bound. Must be in the range of the given dtype.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_INVALID_ARGUMENT Returned when ceiling is invalid for the given dtype.
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when dtype is unsupported.
 *                                       For DTYPE_FLOAT32, use ::vec_ceil_f32() instead.
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note Recommend calling ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_ceil(const vector_t *vec1, vector_t* result, const int ceiling);

/**
 * @brief Element-wise ceiling against an float threshold.
 *
 * Computes @p result[i] = min(@p vec1[i], @p ceiling).
 *
 * @param vec1     Input vector (float dtype).
 * @param result   Output vector; may alias @p vec1.
 * @param ceiling  Float upper bound. 
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when dtype is unsupported.
 *                                       For interger types, use ::vec_ceil() instead.
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note Recommend calling ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_ceil_f32(const vector_t *vec1, vector_t* result, const float ceiling);

/**
 * @brief Element-wise floor against an integer threshold.
 *
 * Computes @p result[i] = max(@p vec1[i], @p floor).
 *
 * @param vec1    Input vector (integer dtypes).
 * @param result  Output vector; may alias @p vec1.
 * @param floor   Integer lower bound. Must be in the range of the given dtype.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_INVALID_ARGUMENT Returned when floor is invalid for the given dtype.
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when dtype is unsupported.
 *                                       For DTYPE_FLOAT32, use ::vec_floor_f32() instead
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note Recommend calling ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_floor(const vector_t *vec1, vector_t* result, const int floor);

/**
 * @brief Element-wise ceiling against an float threshold.
 *
 * Computes @p result[i] = max(@p vec1[i], @p floor).
 *
 * @param vec1     Input vector (float dtype).
 * @param result   Output vector; may alias @p vec1.
 * @param ceiling  Float upper bound. 
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when dtype is unsupported.
 *                                       For interger types, use ::vec_floor() instead.
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note Recommend calling ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_floor_f32(const vector_t *vec1, vector_t* result, const float floor);

/**
 * @brief Saturating negation (handles most-negative value).
 *
 * Computes @p result[i] = -@p vec1[i] with fixups:
 * - INT8: -128 → 127
 * - INT16: INT16_MIN → INT16_MAX
 *
 * @param vec1    Input vector.
 * @param result  Output vector; may alias @p vec1.
 *
 * @retval VECTOR_SUCCESS  
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note INT32 uses standard two’s-complement negation.
 * @note Call ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_neg(const vector_t *vec1, vector_t* result);

/**
 * @brief Multiply-accumulate into a 32-bit accumulator.
 *
 * Computes *@p accumulator += Σ (@p vec1[i] * @p multiplier).
 *
 * @param vec1         Input vector (integer dtypes).
 * @param accumulator  In/out accumulator (32-bit).
 * @param multiplier   Integer multiplier applied to each element.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_TYPE_MISMATCH 
 * @retval VECTOR_UNSUPPORTED_OPERATION Returned when dtype is unsupported.
 *                                       For DTYPE_FLOAT32, use ::vec_mac_f32() instead
 *
 * @note Accumulation is in 32 bits; behavior undefined on overflow.
 * @note Recommend ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_mac(const vector_t *vec1, int32_t* accumulator, const int multiplier);
 
/**
 * @brief Multiply-accumulate into a 32-bit float accumulator.
 *
 * Computes *@p accumulator += Σ (@p vec1[i] * @p multiplier).
 *
 * @param vec1         Input vector (float dtype).
 * @param accumulator  In/out accumulator (32-bit).
 * @param multiplier   Float multiplier applied to each element.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_TYPE_MISMATCH 
 * @retval VECTOR_UNSUPPORTED_OPERATION Returned when dtype is unsupported.
 *                                       For integer dtypes, use ::vec_mac() instead
 *
 * @note Accumulation is in 32 bits; behavior undefined on overflow.
 * @note Recommend ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_mac_f32(const vector_t *vec1, float* accumulator, const float multiplier);

/**
 * @brief Fill a vector with zeros.
 *
 * Sets all elements of @p vec1 to 0 (or 0.0f for FLOAT32).
 *
 * @param vec1  Target vector to fill.
 *
 * @retval VECTOR_SUCCESS  
 *
 * @note Recommend calling ::vector_ok() before mutating a vector.
 */
vector_status_t vec_zeros(vector_t *vec1);

/**
 * @brief Fill a vector with ones.
 *
 * Sets all elements of @p vec1 to 1 (or 1.0f for FLOAT32).
 *
 * @param vec1  Target vector to fill.
 *
 * @retval VECTOR_SUCCESS 
 *
 * @note Recommend calling ::vector_ok() before mutating a vector.
 */
vector_status_t vec_ones(vector_t *vec1);

/**
 * @brief Fill a vector with an integer value.
 *
 * Sets all elements of @p vec1 to @p val.
 * FLOAT32 is not supported in this INT variant and returns VECTOR_UNSUPPORTED_OPERATION.
 * Please use vec_fill_f32()
 *
 * @param vec1  Target vector.
 * @param val   Integer value to write.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when @p vec1->type is DTYPE_FLOAT32;
 *                                       use ::vec_fill_f32() instead. 
 *
 * @note Recommend ::vector_ok() before mutating a vector.
 */
vector_status_t vec_fill(vector_t *vec1, const int val);

/**
 * @brief Fill a vector with an float value.
 *
 * Sets all elements of @p vec1 to @p val.
 * Integer types are not supported in this variant and returns VECTOR_UNSUPPORTED_OPERATION.
 * Please use vec_fill() instead
 *
 * @param vec1  Target vector.
 * @param val   Integer value to write.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_UNSUPPORTED_OPERATION  Returned when @p vec1->type is an integer type;
 *                                       use ::vec_fill() instead. 
 *
 * @note Recommend ::vector_ok() before mutating a vector.
 */
vector_status_t vec_fill_f32(vector_t *vec1, const float val);

/**
 * @brief Creates a deep copy of a vector.
 *
 * Sets all elements of @p result to @p vec1. 
 *
 * @param vec1   Vector to copy.
 * @param result  Target vector.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note Recommend ::vector_ok() before mutating a vector.
 */
vector_status_t vec_copy(vector_t *vec1, vector_t *result);
 

/**
 * @brief Creates a copy-converts a vector from one dtype to another.
 *
 * Sets all elements of @p result to @p vec1. 
 * Currently only supports Widening conversions from INT8 -> INT16/INT32 and INT16 -> INT32.
 * Narrowing conversions and float/integer conversions are not supported and currently return VECTOR_NOT_IMPLEMENTED.
 * Implementation of additional conversions is planned for future releases.
 *
 * @param src   Vector to copy.
 * @param dst   Target vector.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note Recommend ::vector_ok() before mutating a vector.
 */ 
vector_status_t vec_convert(const vector_t *src, vector_t *dst);


#ifdef __cplusplus
}
#endif

#endif
