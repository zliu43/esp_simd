.section .text
.global simd_zeros_i8
.type simd_zeros_i8, @function

/**
 * @brief Zeros an int8_t arr using SIMD.
 *
 * This function uses PIE SIMD instructions and the 128-bit width databus to efficiently zero an arr of 8-bit signed integers.
 * The function is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input arr (int8_t*). 
 * @param a3 Number of elements in the input/output arrs (must be equal for all three).   
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
simd_zeros_i8:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a4, a3, 0, 4                          // a4 = a3 % 16 (remaining tail elements)
    srli a3, a3, 4                              // a3 = a3 / 16 (number of 16-byte SIMD blocks) 
    beqz a3, .Ltail_start                       // if no full blocks, skip SIMD loop

    // SIMD loop for 16-byte blocks  
    ee.xorq q0, q0, q0                          // zeros q0
    loopnez a3, .Lsimd_loop                     // repeat loop a3 times (zero-overhead loop counter)
        ee.vst.128.ip  q0, a2, 16               // store zeros to [a2], post-increment a2 by 16
    .Lsimd_loop:

    .Ltail_start:   

    // Handle remaining elements that are not a multiple of 16
    xor a5, a5, a5                              // zeros a5
    loopnez a4, .Ltail_loop                     // zero overhead loop for scalar tail
        s8i a5, a2, 0                           // store the zero to the address at a2 
        addi a2, a2, 1                          // increments pointer
    .Ltail_loop:  

    movi.n a2, 0                                //return exit code 0 (success)
    retw.n
