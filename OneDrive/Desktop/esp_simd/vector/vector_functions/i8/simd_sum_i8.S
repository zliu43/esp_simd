.section .text
.global simd_sum_i8
.type simd_sum_i8, @function

/**
 * @brief Calculates the sum of an int8_t vector using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently  calculate the sum of a vector of 8-bit signed integers.
 * The addition is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input vector (int8_t*).   
 * @param a3 Pointer to the result 
 * @param a4 Number of elements in the input
 *
 * @return 0 on success.
 *
 * @note The vector pointer (a2) must be 128-bit aligned and the number of elements in a4 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in the input vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_sum_i8:
    entry a1, 16                                    // reserve 16 bytes for the stack frame 
    extui a5, a4, 0, 4                              // extracts the lowest 4 bits of a4 into a5 (a4 % 16), for tail processing
    srli a4, a4, 4                                  // shift a4 right by 4 to get the number of 16-byte blocks (a4 / 16)
    movi.n a6, 0                                    // zeros a6 in case we go straight to the scalar tail
    beqz a4, .Ltail_start                           // if no full blocks (a4 == 0), skip SIMD and go to scalar tail

    // SIMD addition loop for 16-byte blocks 
    movi.n a7, 1                                    // sets a7 to 0x01
    s8i a7, a1, 0                                   // stores 0x01 on the stack
    ee.zero.accx                                    // clears the QACC register
    ee.vld.128.ip     q0, a2, 16                    // loads 16 bytes from a2 into q0, then increment a2 by 16
    ee.vldbc.8.ip     q1, a1, 0                     // broadcast loads the ones vector into q1
    loopnez a4, .Lsimd_loop                         // loop until a4 == 0
        ee.vmulas.s8.accx.ld.ip q0, a2, 16, q0, q1  // multiply-accumulates q0 and q1, stores result in QACC, increments a2, updates q0 
    .Lsimd_loop:

    rur.accx_0 a6                                   // write the lower 32 bits of QACC into a6
    addi a2, a2, -16                                // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    
    // Handle remaining elements that were not part of a full 16-byte block 
    .Ltail_start:
    loopnez a5, .Ltail_loop 
        l8ui a7, a2, 0
        sext a7, a7, 7
        add a6, a7, a6 
        addi a2, a2, 1
    .Ltail_loop:  
        
    s32i.n a6, a3, 0
    movi.n a2,  0                                   //return exit code 0 (success)
    retw.n
