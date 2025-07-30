.section .text
.global simd_dotp_i8
.type simd_dotp_i8, @function

/**
 * @brief Calculates the dot-product of two int8_t vectors using SIMD.
 *
 * This function uses ESP32s3 PIE SIMD instructions to efficiently calculate the dot product of two vectors of 8-bit signed integers.
 * The multiply-accumulate is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input vector (int8_t*).  
 * @param a3 Pointer to the second input vector (int8_t*).
 * @param a4 Pointer to the result 
 * @param a5 Number of elements in the input
 *
 * @return 0 on success.
 *
 * @note The vector pointers (a2, a3) must be 128-bit aligned. The number of elements in a5 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in the input vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_dotp_i8:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 4                              // extracts the lowest 4 bits of a5 into a6 (a5 % 16), for tail processing
    srli a5, a5, 4                                  // shift a5 right by 4 to get the number of 16-byte blocks (a5 / 16)
    movi.n a7, 0                                    // zeros a7
    beqz a5, .Ltail_start                           // if no full blocks (a5 == 0), skip SIMD and go to scalar tail

    // SIMD addition loop for 16-byte blocks 
    ee.zero.accx                                    // clears the QACC register
    ee.vld.128.ip     q0, a2, 16                    // loads 16 bytes from a2 into q0, then increment a2 by 16
    loopnez a5, .Lsimd_loop                         // loop until a5 == 0
        ee.vld.128.ip     q1, a3, 16                // loads 16 bytes from a3 into q1, then increments a3 by 16 
        ee.vmulas.s8.accx.ld.ip q0, a2, 16, q0, q1  // multiply-accumulates q0 and q1, stores result in QACC, increments a2, updates q0 
    .Lsimd_loop:

    rur.accx_0 a7                                   // write the lower 32 bits of QACC into a7
    addi a2, a2, -16                                // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    .Ltail_start:
    // Handle remaining elements that were not part of a full 16-byte block 
 
    loopnez a6, .Ltail_loop 
        l8ui a8, a2, 0
        sext a8, a8, 7
        l8ui a9, a3, 0
        sext a9, a9, 7
        mull a8, a8, a9
        add a7, a7, a8 
        addi a2, a2, 1
        addi a3, a3, 1
    .Ltail_loop:  
        
    s32i.n a7, a4, 0
    movi.n a2,  0                                   //return exit code 0 (success)
    retw.n
