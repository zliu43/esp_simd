.section .text
.global simd_mul_scalar_f32
.type simd_mul_scalar_f32, @function

/**
 * @brief Performs scalar multiplication of a 32 bit float vector.
 *
 * This function uses the 128-bit data to process 4 elements at a time. Any remaining elements are processed using a scalar tail.
 *
 * @param a2 Pointer to the first input vector (float*).
 * @param a3 Pointer to the scalar value (float*).
 * @param a4 Pointer to the output/result vector (float*). 
 * @param a5 Number of elements in the input/output vectors (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vector pointers (a2, a4) must be 128-bit aligned and the number of elements in a5 must be a multiple of 4
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_mul_scalar_f32: 
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a6, a5, 0, 2                          // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4)

    lsip f4, a3, 0
    beqz a5, .Ltail_start
    
    ee.ldf.128.ip f3, f2, f1, f0, a2, 16        // loads first loop  
    mul.s f8, f3, f4                            // precomputes first cycle
    mul.s f7, f2, f4 
    mul.s f6, f1, f4 
    mul.s f5, f0, f4 

    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.ldf.128.ip f3, f2, f1, f0, a2, 16    // load 4 elements from vec1, increment a2 
        ee.stf.128.ip f8, f7, f6, f5, a4, 16    // store result
        mul.s f8, f3, f4 
        mul.s f7, f2, f4 
        mul.s f6, f1, f4 
        mul.s f5, f0, f4 
    .Lsimd_loop:
 
    addi a2, a2, -16                            // moves pointer back

    .Ltail_start:
    loopnez a6, .Ltail_loop
        lsip f0, a2, 4                          // load the first element, increment a2 
        mul.s f0, f0, f4                        // performs f32 addition
        ssip f0, a4, 4                          // stores the result, increments a4
    .Ltail_loop:
 
    movi.n a2, 0                                // return exit code 0 (success)
    retw.n