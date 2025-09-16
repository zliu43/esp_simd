.section .text
.global simd_mac_i16
.type simd_mac_i16, @function

/**
 * @brief Multiply-accumulates the produce of an int16_t scalar and an int16_t arr using SIMD and adds the sum to the given value.
 *
 * This function uses PIE SIMD instructions to efficiently calculate the sum of a vector of 16-bit signed integers.
 * The addition is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input arr (int16_t*).
 * @param a3 Pointer to the accumulator (int32_t*)
 * @param a4 Multiplier value (int16_t)
 * @param a5 Number of elements in the input
 *
 * @return 0 on success.
 *
 * @note The arr pointer (a2) must be 128-bit aligned and the number of elements in a5 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in the input vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_mac_i16:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 3                              // extracts the lowest 4 bits of a5 into a6 (a5 % 8), for tail processing
    srli a5, a5, 3                                  // shift a5 right by 4 to get the number of 16-byte blocks (a5 / 8)
    movi.n a7, 0                                    // zeros a7 in case we go straight to the scalar tail
    beqz a5, .Ltail_start                           // if no full blocks (a4 == 0), skip SIMD and go to scalar tail

    // SIMD addition loop for 16-byte blocks  S
    ee.zero.accx                                    // clears the QACC register
    ee.vld.128.ip     q0, a2, 16                    // loads 16 bytes from a2 into q0, then increment a2 by 16
    ee.vldbc.16.ip    q1, a4, 0                     // broadcast loads the multiplier into q1
    loopnez a5, .Lsimd_loop                         // loop until a5 == 0
        ee.vmulas.s16.accx.ld.ip q0, a2, 16, q0, q1 // multiply-accumulates q0 and q1, stores result in QACC, increments a2, updates q0
    .Lsimd_loop:

    rur.accx_0 a7                                   // write the lower 32 bits of QACC into a7
    addi a2, a2, -16                                // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    // Handle remaining elements that were not part of a full 16-byte block  
    .Ltail_start:    

    l16si a5, a4, 0 

    loopnez a6, .Ltail_loop
        l16si a8, a2, 0 
        mull a8, a8, a5
        add a7, a8, a7
        addi a2, a2, 2
    .Ltail_loop:
    
    l32i a9, a3, 0                                  // load the accumlator value
    add a7, a9, a7                                  // add the current result to the accumulator
    s32i.n a7, a3, 0                                // stores the accumulated value
    
    movi.n a2,  0                                   // return exit code 0 (success)
    retw.n
