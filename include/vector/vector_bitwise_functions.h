#ifndef VECTOR_BITWISE_FUNCTIONS_H
#define VECTOR_BITWISE_FUNCTIONS_H

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Element-wise XOR.
 *
 * Computes @p result[i] = @p vec1[i] ^ @p vec2[i] for integer dtypes. 
 *
 * @param vec1 - input vector
 * @param vec2 - input vector; may alias @p vec1
 * @param result - output vector; @p result may alias inputs.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 *
 * @note Recommend ::vector_ok() before bitwise operations.
 */
vector_status_t vec_xor(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Element-wise AND.
 *
 * Computes @p result[i] = @p vec1[i] & @p vec2[i] for integer dtypes. 
 *
 * @param vec1 - input vector
 * @param vec2 - input vector; may alias @p vec1
 * @param result - output vector; @p result may alias inputs.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 */
vector_status_t vec_and(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Element-wise OR.
 *
 * Computes @p result[i] = @p vec1[i] | @p vec2[i] for integer dtypes. 
 *
 * @param vec1 - input vector
 * @param vec2 - input vector; may alias @p vec1
 * @param result - output vector; @p result may alias inputs.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH 
 */
vector_status_t vec_or(const vector_t *vec1, const vector_t *vec2, vector_t *result);

/**
 * @brief Bitwise NOT.
 *
 * Computes @p result[i] = ~@p vec1[i] for integer dtypes. 
 *
 * @param vec1    Input vector.
 * @param result  Output vector; may alias @p vec1.
 *
 * @retval VECTOR_SUCCESS
 * @retval VECTOR_SIZE_MISMATCH
 * @retval VECTOR_TYPE_MISMATCH  
 *
 * @note Recommend ::vector_ok() before operation.
 */
vector_status_t vec_not(const vector_t *vec1, vector_t *result);

 
#ifdef __cplusplus
}
#endif

#endif
