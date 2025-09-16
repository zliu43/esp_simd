.section .text
.global simd_sum_i32
.type simd_sum_i32, @function

/**
 * @brief Calculates the sum of an int32_t vector.
 *
 * This function calculates ∑a[i], processing 4 elements per loop iteration using SIMD instructions.
 * Any remaining elements (if the length is not a multiple of 4) are handled sequentially. 
 *
 * @param a2 Pointer to the first input vector (int32_t*).
 * @param a3 Pointer to the result (int32_t*).
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
 * @warning Signed integer overflow is undefined behavior. If the sum of the input elements exceeds
 *          the range of int32_t ([-2,147,483,648, 2,147,483,647]), the result is unpredictable.
 */
simd_sum_i32:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a5, a4, 0, 2                              // extracts the lowest 2 bits of a4 into a5 (a4 % 4), for tail processing
    srli a4, a4, 2                                  // shift a4 right by 2 to get the number of 16-byte blocks (a4 / 4)
    movi.n a6, 0                                    // zeros a6 in case we go straight to the scalar tail 
    beqz a4, .Ltail_start                           // if no full blocks, skip SIMD loop
    
    ee.xorq q0, q0, q0                              // clears q0 to use for accumulation
    ee.vld.128.ip q1, a2, 16                        // loads elements of vector, increments pointer by 16
    loopnez a4, .Lsimd_loop                         // loop until a4 == 0 
        ee.vadds.s32.ld.incp q1, a2, q0, q1, q0     // adds elements to q0, addition saturates 32 bit lanes
    .Lsimd_loop: 
    
    ee.movi.32.a q0, a6, 0                          // horizontal sum of q0 into a6
    ee.movi.32.a q0, a7, 1
    ee.movi.32.a q0, a8, 2
    ee.movi.32.a q0, a9, 3
    add.n a6, a6, a7
    add.n a6, a6, a8
    add.n a6, a6, a9 

    addi a2, a2, -16                                // moves a2 back (it goes 16 bytes to far in the loop)

    .Ltail_start:

    loopnez a5, .Ltail_loop 
        l32i.n a7, a2, 0 
        add.n a6, a7, a6 
        addi.n a2, a2, 4
    .Ltail_loop:  
        
    s32i.n a6, a3, 0
    movi.n a2,  0                                   //return exit code 0 (success)
    retw.n
