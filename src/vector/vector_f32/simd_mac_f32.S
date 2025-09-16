.section .text
.global simd_mac_f32
.type simd_mac_f32, @function

/**
 * @brief Multiply-accumulates the product of an float scalar and an float arr and writes back the sum.
 *
 * This function calculates *a3 += ∑a2[i] * *a4
 *
 * @param a2 Pointer to the first input arr (float*).
 * @param a3 Pointer to the accumulator (float*)
 * @param a4 Pointer to the multiplier value (float*)
 * @param a5 Number of elements in the input
 *
 * @return 0 on success.
 *  
 * @pre All input pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in the input vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_mac_f32:
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a6, a5, 0, 2                          // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4)
    
    lsip f4, a3, 0 
    lsip f5, a4, 0
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.ldf.128.ip f3, f2, f1, f0, a2, 16    // load 4 elements from vec1, increment a2 
        madd.s f4, f3, f5                       // multiply elements
        madd.s f4, f2, f5 
        madd.s f4, f1, f5 
        madd.s f4, f0, f5  
    .Lsimd_loop:
 
    loopnez a6, .Ltail_loop
        lsip f0, a2, 4                          // load the first element, increment a2 
        madd.s f4, f0, f5   
    .Ltail_loop:

    ssi f4, a3, 0                               // stores the result, 
 
    movi.n a2, 0                                // return VECTOR_SUCCESS
    retw.n

