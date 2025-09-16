.section .text
.global simd_abs_i16
.type simd_abs_i16, @function

/**
 * @brief Performs element-wise absolute value of an int16_t arr using SIMD. -32768 returns as 32767.
 *
 * This function uses PIE SIMD instructions to efficiently absolute value an arr of 16-bit signed integers.
 * The addition is performed in parallel across 128-bit vector registers, processing 8 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 8) are handled sequentially.
 *
 * @param a2 Pointer to the first input arr (int16_t*).
 * @param a3 Pointer to the ouptut arr (int16_t*).
 * @param a4 Number of elements in the input/output arrs (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All arr pointers (a2, a3) must be 128-bit aligned and the number of elements in a4 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_abs_i16:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a5, a4, 0, 3                          // a5 = a4 % 8 (remaining tail elements)
    srli a4, a4, 3                              // a4 = a4 / 8 (number of 16-byte SIMD blocks)
    movi a7, -32767                             // a7 = -32767, for replacing -32768 edge cases
    beqz a4, .Ltail_start                       // if no full blocks, skip SIMD loop

    // SIMD loop for 16-byte blocks 
    ssai 0                                      // Clears Shift Amount Register
    s16i a7, a1, 0                               // store -32767 to stack (for broadcast loading)
    ee.vld.128.ip   q0, a2, 16                  // load 16 bytes from [a2] into q0, post-increment a2 by 16
    ee.vcmp.eq.s16   q1, q1, q1                 // fill q1 with all 0xFF (-1), used as multiplier for negation
    ee.vldbc.16.ip   q2, a1, 0                  // broadcast-load -32767 from stack into all lanes of q2
    loopnez a4, .Lsimd_loop                     // repeat loop a4 times (zero-overhead loop counter)
        ee.vmax.s16.ld.incp  q0, a2, q4, q0, q2 // q4 = max(q0, -32767); replace -32768 with -32767, load next 16 bytes into q0, post-increment a2
        ee.vmul.s16 q3, q4, q1                  // q3 = q4 * -1; negates all q4 values
        ee.vmax.s16 q4, q4, q3                  // q4 = max(q4, q3); effectively performs abs(q4)
        ee.vst.128.ip  q4, a3, 16               // store result q4 to [a3], post-increment a3 by 16
    .Lsimd_loop:

    addi a2, a2, -16                            // adjust a2 pointer back to the last processed element  

    .Ltail_start:    
    // Handle remaining elements that are not a multiple of 16
    loopnez a5, .Ltail_loop
        l16si a8, a2, 0                          // loads and sign-extends the element into a8 
        
        max a8, a8, a7                          // sets a8 to -32767 if it is -32768
        abs a8, a8                              // takes the abs of a8
 
        s16i a8, a3, 0                          // store the saturated result in address at a4

        addi.n a2, a2, 2                        // increment pointers 
        addi.n a3, a3, 2 
    .Ltail_loop:  

    movi.n a2, 0                                // return exit code 0 (success)
    retw.n
