.section .text
.global simd_dotp_f32
.type simd_dotp_f32, @function

/**
 * @brief Calculates the dot-product of two float vectors.
 * 
 * Any remaining elements (if the length is not a multiple of 4) are handled sequentially.
 *
 * @param a2 Pointer to the first input vector (float*).
 * @param a3 Pointer to the second input vector (float*).
 * @param a4 Pointer to the result (float*)
 * @param a5 Number of elements in the input
 *
 * @return 0 on success.
 *   
 * @pre The size in a5 must match the number of elements in the input vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_dotp_f32: 
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a6, a5, 0, 2                          // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4)
   
    sub.s f8, f8, f8                            // zeros f8 for accumulator
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.ldf.128.ip f3, f2, f1, f0, a2, 16    // load 4 elements from vec1, increment a2
        ee.ldf.128.ip f7, f6, f5, f4, a3, 16    // load 4 elements from vec2, increment a3
        madd.s f8, f3, f7                       // multiply elements
        madd.s f8, f2, f6 
        madd.s f8, f1, f5 
        madd.s f8, f0, f4                       // accumulate
    .Lsimd_loop:
 
    loopnez a6, .Ltail_loop
        lsip f0, a2, 4                          // load the first element, increment a2
        lsip f1, a3, 4                          // load the second element, increment a3
        madd.s f8, f0, f1                        // performs f32 multiplication 
    .Ltail_loop:
    ssi f8, a4, 0
 
    movi.n a2, 0                                // return VECTOR_SUCCESS
    retw.n