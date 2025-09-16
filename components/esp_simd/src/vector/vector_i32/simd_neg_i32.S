.section .text
.global simd_neg_i32
.type simd_neg_i32, @function

/**
 * @brief Performs element-wise negation of an int32_t arr using SIMD. 
 *
 * This function uses PIE SIMD instructions to efficiently negate a arr of 32-bit signed integers.
 * The negation is performed in parallel across 128-bit vector registers, processing 4 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input arr (int32_t*).
 * @param a3 Pointer to the ouptut arr (int32_t*).
 * @param a4 Number of elements in the input/output arrs (must be equal for both).   
 *
 * @return 0 on success.
 *
 * @note All arr pointers (a2, a3) must be 128-bit aligned and the number of elements in a4 must be a multiple of 4
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_neg_i32:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a5, a4, 0, 2                          // a5 = a4 % 4 (remaining tail elements)
    srli a4, a4, 2                              // a4 = a4 / 4 (number of 16-byte SIMD blocks)  

    // SIMD loop for 16-byte blocks   
    loopnez a4, .Lsimd_loop                     // repeat loop a4 times (zero-overhead loop counter)
        l32i.n a7, a2, 0
        l32i.n a8, a2, 4
        l32i.n a9, a2, 8
        l32i.n a10, a2, 12
        neg a7, a7 
        neg a8, a8 
        neg a9, a9 
        neg a10, a10 
        s32i.n a7, a3, 0
        s32i.n a8, a3, 4
        s32i.n a9, a3, 8
        s32i.n a10, a3, 12
        addi a2, a2, 16
        addi a3, a3, 16 
    .Lsimd_loop: 
    // Handle remaining elements that are not a multiple of 16
    loopnez a5, .Ltail_loop
        l32i.n a8, a2, 0                        // loads and sign-extends the element into a8 
        neg a8, a8
        s32i.n a8, a3, 0                        // store the saturated result in address at a3 
        addi.n a2, a2, 4                        // increment pointers 
        addi.n a3, a3, 4 
    .Ltail_loop:  

    movi.n a2, 0                                // return exit code 0 (success)
    retw.n
