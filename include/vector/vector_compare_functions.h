#ifndef VECTOR_COMPARE_FUNCTIONS_H
#define VECTOR_COMPARE_FUNCTIONS_H

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif
 
/**
 * @brief Element-wise greater-than comparison.
 *
 * Produces an mask vector comparing @p vec1[i] > @p vec2[i].
 * Each result element is 0 for false, -1 for true.
 *
 * @param vec1    Left operand.
 * @param vec2    Right operand.
 * @param result  Output mask vector.
 *
 * @retval VECTOR_SUCCESS  
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 * @retval VECTOR_NOT_IMPLEMENTED Float type currently not supported.
 *
 * @note Recommend ::vector_ok() before comparisons.
 */
vector_status_t vec_gt(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Element-wise less-than comparison.
 *
 * Produces an mask vector comparing @p vec1[i] < @p vec2[i].
 * Each result element is 0 for false, -1 for true.
 *
 * @param vec1    Left operand.
 * @param vec2    Right operand.
 * @param result  Output mask vector.
 *
 * @retval VECTOR_SUCCESS  
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 * @retval VECTOR_NOT_IMPLEMENTED Float type currently not supported.
 * 
 * @note Recommend ::vector_ok() before comparisons.
 */
vector_status_t vec_lt(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Element-wise equality comparison.
 *
 * Produces an mask vector comparing @p vec1[i] == @p vec2[i].
 * Each result element is 0 for false, -1 for true.
 *
 * @param vec1    Left operand.
 * @param vec2    Right operand.
 * @param result  Output mask vector.
 *
 * @retval VECTOR_SUCCESS  
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 * @retval VECTOR_NOT_IMPLEMENTED Float type currently not supported.
 * 
 * @note Recommend ::vector_ok() before comparisons.
 */
vector_status_t vec_eq(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Element-wise maximum of two vectors.
 *
 * Computes @p result[i] = max(@p vec1[i], @p vec2[i]). 
 *
 * @param vec1, vec2, result  Operands and output; @p result may alias inputs.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH  
 * @retval VECTOR_NOT_IMPLEMENTED Float type currently not supported.
 *  
 * @note Recommend ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_max(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Element-wise minimum of two vectors.
 *
 * Computes @p result[i] = min(@p vec1[i], @p vec2[i]). 
 *
 * @param vec1, vec2, result  Operands and output; @p result may alias inputs.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH  
 * @retval VECTOR_NOT_IMPLEMENTED Float type currently not supported.
 *
 * @note Recommend ::vector_ok() before arithmetic operations.
 */
vector_status_t vec_min(const vector_t *vec1, const vector_t *vec2, vector_t *result);

 
#ifdef __cplusplus
}
#endif

#endif
