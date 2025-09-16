.section .text
.global simd_xor_i16
.type simd_xor_i16, @function

/**
 * @brief Performs element-wise xor of two int16_t vectors using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently xor two vectors of 16-bit signed integers.
 * The xor operation is performed in parallel across 128-bit vector registers, processing 8 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 8) are handled sequentially.
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
simd_xor_i16:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 3                              // extracts the lowest 3 bits of a5 into a6 (a5 % 8), for tail processing
    srli a5, a5, 3                                  // shift a5 right by 3 to get the number of 16-byte blocks (a5 / 8)

    // SIMD xor loop for 16-byte blocks
    loopnez a5, .Lsimd_loop                         // loop until a5 == 0
        ee.vld.128.ip     q0, a2, 16                // loads 16 bytes from a2 into q0, then increment a2 by 16
        ee.vld.128.ip     q1, a3, 16                // loads 16 bytes from a3 into q1, increment a3 by 16
        ee.xorq           q4, q0, q1                // performs element-wise xor of q0 and q1, result in q4
        ee.vst.128.ip     q4, a4, 16                // stores 16 bytes from q4 to address at a4, increment a4 by 16
    .Lsimd_loop:
  
    // Handle remaining elements that were not part of a full 16-byte block  
    loopnez a6, .Ltail_loop
        l16ui a7, a2, 0                              // loads the elements of the two vectors
        l16ui a8, a3, 0

        xor a7, a7, a8                              // perform xor operation
        s16i a7, a4, 0                              // store the result in address at a4

        addi.n a2, a2, 2                            //increment pointers
        addi.n a3, a3, 2
        addi.n a4, a4, 2 
    .Ltail_loop:

    movi.n a2, 0 //return exit code 0 (success)
    retw.n
