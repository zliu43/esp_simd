#ifndef VECTOR_EXTRA_FUNCTIONS_H
#define VECTOR_EXTRA_FUNCTIONS_H
 
#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif
  
/**
 * @brief ReLU with scale and post-shift (fixed-point). INT32 and float datatypes currently not supported.
 *
 * Computes @p result[i] = max(0, @p vec1[i]) * @p multiplier >> @p shift_amount.
 * SHIFT is a logical bitwise right shift.
 *
 * @param vec1          Input vector
 * @param result        Output vector; may alias @p vec1.
 * @param multiplier    Integer scaling factor applied after ReLU.
 * @param shift_amount  Bits to shift after scaling.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Unsupported dtype.
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note INT8/INT16 use saturating arithmetic; INT32 does not.
 * @note Recommend ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_relu(const vector_t *vec1, vector_t* result, const int multiplier, const unsigned int shift_amount);


/**
 * @brief Element-wise multiply with widening for integer types.
 *
 * - INT8 × INT8 → INT16 output
 * - INT16 × INT16 → INT32 output 
 *
 * INT8/INT16 are SIMD-accelerated;  
 *
 * @param vec1    Left operand.
 * @param vec2    Right operand.
 * @param result  Output vector; dtype must match the widened or same-type result as above.
 *
 * @retval VECTOR_SUCCESS 
 * @retval VECTOR_UNSUPPORTED_OPERATION  Dtype combination unsupported.
 * @retval VECTOR_SIZE_MISMATCH  
 * 
 * @note Recommend calling ::vector_ok() on all vectors first.
 * @pre Operands must have identical dtype and size; result must match required output dtype.
 */
vector_status_t vec_mul_widen(const vector_t *vec1, const vector_t *vec2, vector_t *result); 
 
#ifdef __cplusplus
}
#endif

#endif
