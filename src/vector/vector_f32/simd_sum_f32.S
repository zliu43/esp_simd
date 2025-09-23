.section .text
.global simd_sum_f32
.type simd_sum_f32, @function

/**
 * @brief Calculates the sum of an float* vector.
 *
 * This function calculates ∑a[i], processing 4 elements per loop iteration using SIMD instructions.
 * Any remaining elements (if the length is not a multiple of 4) are handled sequentially. 
 *
 * @param a2 Pointer to the first input vector (float*).
 * @param a3 Pointer to the result (float*).
 * @param a4 Number of elements in the input.
 *
 * @return 0 on success.
 *
 * @note The vector pointer (a2) must be 128-bit aligned and the number of elements in a4 must be a multiple of 4
 *       for efficient processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in the input vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions. 
 */
simd_sum_f32: 
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a5, a4, 0, 2                          // extracts the lowest 2 bits of a4 into a5 (a4 % 4), for tail processing
    srli a4, a4, 2                              // shift a4 right by 2 to get the number of 16-byte blocks a4 = (a4 / 4)
    
    const.s f4, 0
    const.s f5, 0
    const.s f6, 0
    const.s f7, 0

    loopnez a4, .Lsimd_loop                     // loop until a4 == 0
        ee.ldf.128.ip f3, f2, f1, f0, a2, 16    // load 4 elements from vec1, increment a2 
        add.s f4, f0, f4
        add.s f5, f1, f5
        add.s f6, f2, f6
        add.s f7, f3, f7 
    .Lsimd_loop:

    add.s f4, f4, f5
    add.s f6, f6, f7
    add.s f4, f4, f6
 
    loopnez a5, .Ltail_loop
        lsip f0, a2, 4                          // load the first element, increment a2 
        add.s f4, f0, f4                        // performs f32 addition
    .Ltail_loop:

    ssi f4, a3, 0                              // stores the result, 
 
    movi.n a2, 0                                // return exit code 0 (success)
    retw.n
