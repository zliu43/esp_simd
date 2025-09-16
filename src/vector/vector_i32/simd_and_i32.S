.section .text
.global simd_and_i32
.type simd_and_i32, @function

/**
 * @brief Perform element-wise bitwise AND on two int32_t arrays.
 *
 * Computes C[i] = A[i] & B[i] for each element in the input arrays.
 * Processes data in 16-byte (4 × int32_t) blocks using a zero-overhead loop
 * for improved throughput, followed by a scalar loop for any remaining elements.
 *
 * @param a2 Pointer to the first input vector (int16_t*).
 * @param a3 Pointer to the second input vector (int16_t*).
 * @param a4 Pointer to the output/result vector (int16_t*).
 * @param a5 Number of elements in the input/output vectors (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vector pointers (a2, a3, a4) must be 128-bit aligned and the number of elements in a5 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_and_i32:
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a14, a5, 0, 2                         // a14 = a5 % 4 (tail elements)
    srli a5, a5, 2                              // a5 = a5 / 4 (16-byte blocks)

    // Process 16-byte chunks (4 x int32 per iteration)
    loopnez a5, .Lsimd_loop                         // loop until a5 == 0
        ee.vld.128.ip     q0, a2, 16                // loads 16 bytes from a2 into q0, then increment a2 by 16
        ee.vld.128.ip     q1, a3, 16                // loads 16 bytes from a3 into q1, increment a3 by 16
        ee.andq           q4, q0, q1                // performs element-wise and of q0 and q1, result in q4
        ee.vst.128.ip     q4, a4, 16                // stores 16 bytes from q4 to address at a4, increment a4 by 16
    .Lsimd_loop:

    // Tail loop for remaining (a14) elements
    loopnez a14, .Ltail_loop
        l32i.n a6,  a2, 0
        l32i.n a10, a3, 0
        addi.n a2, a2, 4
        addi.n a3, a3, 4
        and a6, a6, a10
        s32i.n a6, a4, 0
        addi.n a4, a4, 4
    .Ltail_loop:

    movi.n a2, 0                                // return 0 (success)
    retw.n
