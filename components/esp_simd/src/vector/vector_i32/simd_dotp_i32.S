.section .text
.global simd_dotp_i32
.type simd_dotp_i32, @function

/**
 * @brief Calculates the dot-product of two int32_t vectors.
 * 
 * Any remaining elements (if the length is not a multiple of 4) are handled sequentially.
 *
 * @param a2 Pointer to the first input vector (int32_t*).
 * @param a3 Pointer to the second input vector (int32_t*).
 * @param a4 Pointer to the result
 * @param a5 Number of elements in the input
 *
 * @return 0 on success.
 *   
 * @pre The size in a5 must match the number of elements in the input vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_dotp_i32:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 2                              // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                                  // shift a5 right by 2 to get the number of 16-byte blocks (a5 / 4)  
    movi.n a11, 0                                   // 


    // SIMD addition loop for 16-byte blocks  
    loopnez a5, .Lsimd_loop
        l32i.n a7, a2, 0
        l32i.n a8, a3, 0
        mull a7, a7, a8
        l32i.n a9, a2, 4
        l32i.n a10, a3, 4
        add.n a11, a7, a11
        mull a9, a9, a10
        l32i.n a7, a2, 8
        l32i.n a8, a3, 8
        add.n a11, a9, a11
        mull a7, a7, a8
        l32i.n a9, a2, 12
        l32i.n a10, a3, 12
        add.n a11, a7, a11
        mull a9, a9, a10
        addi a2, a2, 16
        addi a3, a3, 16
        add.n a11, a9, a11 
    .Lsimd_loop:

    /**  Original Assimbly code before interweaving for interlock / mull latency

        # i = 0
        l32i.n  a7,  a2, 0
        l32i.n  a8,  a3, 0
        mull    a7,  a7, a8
        add.n   a11, a7, a11

        # i = 1
        l32i.n  a9,  a2, 4
        l32i.n  a10, a3, 4
        mull    a9,  a9, a10
        add.n   a11, a9, a11

        # i = 2
        l32i.n  a7,  a2, 8
        l32i.n  a8,  a3, 8
        mull    a7,  a7, a8
        add.n   a11, a7, a11

        # i = 3
        l32i.n  a9,  a2, 12
        l32i.n  a10, a3, 12
        mull    a9,  a9, a10
        add.n   a11, a9, a11

        addi    a2, a2, 16
        addi    a3, a3, 16 */
     
    // Handle remaining elements that were not part of a full 16-byte block 
 
    loopnez a6, .Ltail_loop 
        l32i.n a8, a2, 0
        l32i.n a9, a3, 0 
        mull a8, a8, a9
        addi.n a2, a2, 4
        addi.n a3, a3, 4
        add.n a11, a11, a8
    .Ltail_loop:
    
    s32i.n a11, a4, 0
    movi.n a2,  0                                   //return exit code 0 (success)
    retw.n
