.section .text
.global simd_copy_i16
.type simd_copy_i16, @function

/**
 * @brief Creates a copy of an int16_t vectors using PIE SIMD and the 128 bit databus.
 *
 * This function uses PIE SIMD instructions to efficiently copy a vector of 16-bit signed integers. 
 * Any remaining elements (if the length is not a multiple of 8) are handled sequentially. 
 *
 * @param a2 Pointer to the first input vector (int16_t*). 
 * @param a3 Pointer to the output/result vector (int16_t*).
 * @param a4 Number of elements in the input/output vectors (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vector pointers (a2, a3) must be 128-bit aligned and the number of elements in a5 must be a multiple of 8
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_copy_i16:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a5, a4, 0, 3                              // extracts the lowest 3 bits of a4 into a5 (a4 % 8), for tail processing
    srli a4, a4, 3                                  // shift a4 right by 3 to get the number of 16-byte blocks (a4 / 8) 
 
    loopnez a4, .Lsimd_loop                         // loop until a4 == 0
        ee.vld.128.ip     q1, a2, 16                // loads 16 bytes from a2 into q1, increment a32 by 16 
        ee.vst.128.ip     q1, a3, 16                // stores 16 bytes from q1 to address at a3, increment a3 by 16
    .Lsimd_loop: 
    
    loopnez a5, .Ltail_loop                         // Handle remaining elements that are not a multiple of 8
        l16ui a7, a2, 0   
        addi.n a2, a2, 2                            // increment pointers
        s16i a7, a3, 0                              // store the saturated result in address at a3 
        addi.n a3, a3, 2                            // increment pointers 
    .Ltail_loop:  

    movi.n a2, 0                                    // return exit code 0 (success)
    retw.n
