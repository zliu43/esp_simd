.section .text
.global simd_ones_i32
.type simd_ones_i32, @function

/**
 * @brief Ones an int32_t arr using SIMD.
 *
 * This function uses PIE SIMD instructions and the 128-bit width databus to efficiently zero an arr of 32-bit signed integers.
 * The function is performed in parallel across 128-bit vector registers, processing 4 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 8) are handled sequentially.
 *
 * @param a2 Pointer to the first input arr (int32_t*). 
 * @param a3 Number of elements in the output arr.
 *
 * @return 0 on success.
 *
 * @note All arr pointers (a2) must be 128-bit aligned and the number of elements in a3 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size a3 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_ones_i32:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a4, a3, 0, 2                          // a4 = a3 % 4 (remaining tail elements)
    srli a3, a3, 2                              // a3 = a3 / 4 (number of 8-byte SIMD blocks) 
    movi.n a6, 1                                // a6 = 1 
    s32i.n a6, a1, 0                            // store 1 to stack (for broadcast loading) 

    // SIMD loop for 16-byte blocks  
    ee.vldbc.32.ip   q0, a1, 0                  // broadcast-load 1 from stack into all lanes of q0
    loopnez a3, .Lsimd_loop                     // repeat loop a3 times (zero-overhead loop counter)
        ee.vst.128.ip  q0, a2, 16               // store ones to [a2], post-increment a2 by 16
    .Lsimd_loop:
 
    // Handle remaining elements that are not a multiple of 16 
    loopnez a4, .Ltail_loop                     // zero overhead loop for scalar tail
        s32i.n a6, a2, 0                        // store the value to the address at a2 
        addi.n a2, a2, 4                        // increments pointer
    .Ltail_loop:  

    movi.n a2, 0                                // return exit code 0 (success)
    retw.n
