.section .text
.global simd_not_i32
.type simd_not_i32, @function

/**
 * @brief Performs element-wise not of two int32_t vectors using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently not two vectors of 32-bit signed integers.
 * The not operation is performed in parallel across 128-bit vector registers, processing 4 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 4) are handled sequentially.
 *
 * @param a2 Pointer to the first input vector (int32_t*). 
 * @param a3 Pointer to the output/result vector (int32_t*).
 * @param a4 Number of elements in the input/output vectors (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vector pointers (a2, a3) must be 128-bit aligned and the number of elements in a4 must be a multiple of 4
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_not_i32:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a5, a4, 0, 2                              // extracts the lowest 2 bits of a4 into a5 (a4 % 4), for tail processing
    srli a4, a4, 2                                  // shift a4 right by 2 to get the number of 16-byte blocks (a4 / 4)

    // SIMD not loop for 16-byte blocks
    loopnez a4, .Lsimd_loop                         // loop until a4 == 0
        ee.vld.128.ip     q0, a2, 16                // loads 16 bytes from a2 into q0, then increment a2 by 16 
        ee.notq           q4, q0                    // performs element-wise not of q0, result in q4
        ee.vst.128.ip     q4, a3, 16                // stores 16 bytes from q4 to address at a3, increment a3 by 16
    .Lsimd_loop:
  
    
    movi a8, -1                                     // Handle remaining elements that were not part of a full 16-byte block
    loopnez a5, .Ltail_loop
        l32i.n a7, a2, 0                            // loads the elements of the two vectors 
        addi.n a2, a2, 4
        xor a7, a7, a8                              // perform not operation
        s32i.n a7, a3, 0                            // store the result in address at a4 
        addi.n a3, a3, 4
    .Ltail_loop:

    movi.n a2, 0                                    // return exit code 0 (success)
    retw.n
