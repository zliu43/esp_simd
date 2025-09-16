.section .text
.global simd_mac_i32
.type simd_mac_i32, @function

/**
 * @brief Multiply-accumulates the produce of an int32_t scalar and an int32_t arr using SIMD and adds the sum to the given value.
 *
 * This function calculates the 
 *
 * @param a2 Pointer to the first input arr (int32_t*).
 * @param a3 Pointer to the accumulator (int32_t*)
 * @param a4 Pointer to the multiplier value (int32_t*)
 * @param a5 Number of elements in the input
 *
 * @return 0 on success.
 *
 * @note Integer overflow is not detected
 *
 * @pre All input pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in the input vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_mac_i32:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a12, a5, 0, 2                              // extracts the lowest 4 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                                  // shift a5 right by 4 to get the number of 16-byte blocks (a5 / 4)
    movi.n a7, 0                                    // zeros a7  
    l32i.n a6, a4, 0 
 
    loopnez a5, .Lsimd_loop   
        l32i a8, a2, 0
        l32i a9, a2, 4
        l32i a10, a2, 8
        l32i a11, a2, 12
        mull a8, a8, a6
        mull a9, a9, a6
        mull a10, a10, a6
        mull a11, a11, a6
        add.n a7, a7, a8
        add.n a7, a7, a9
        add.n a7, a7, a10
        add.n a7, a7, a11
        addi a2, a2, 16
    .Lsimd_loop: 

    loopnez a12, .Ltail_loop
        l32i.n a8, a2, 0 
        mull a8, a8, a6
        add.n a7, a8, a7
        addi.n a2, a2, 4
    .Ltail_loop:
    
    l32i.n a9, a3, 0                                // load the accumlator value
    add.n a7, a9, a7                                // add the current result to the accumulator
    s32i.n a7, a3, 0                                // stores the accumulated value
    
    movi.n a2,  0                                   // return exit code 0 (success)
    retw.n
