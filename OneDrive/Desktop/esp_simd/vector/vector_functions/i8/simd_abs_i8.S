.section .text
.global simd_abs_i8
.type simd_abs_i8, @function

/**
 * @brief Performs element-wise absolute value of an int8_t arr using SIMD. -128 returns as 127.
 *
 * This function uses PIE SIMD instructions to efficiently absolute value an arr of 8-bit signed integers.
 * The addition is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input arr (int8_t*).
 * @param a3 Pointer to the ouptut arr (int8_t*). 
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
simd_abs_i8:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a5, a4, 0, 4                          // a5 = a4 % 16 (remaining tail elements)
    srli a4, a4, 4                              // a4 = a4 / 16 (number of 16-byte SIMD blocks)
    movi a7, -127                               // a7 = -127, for replacing -128 edge cases
    beqz a4, .Ltail_start                       // if no full blocks, skip SIMD loop

    // SIMD loop for 16-byte blocks 
    ssai 0                                      // Clears Shift Amount Register
    s8i a7, a1, 0                               // store -127 to stack (for broadcast loading)
    ee.vld.128.ip   q0, a2, 16                  // load 16 bytes from [a2] into q0, post-increment a2 by 16
    ee.vcmp.eq.s8   q1, q1, q1                  // fill q1 with all 0xFF (-1), used as multiplier for negation
    ee.vldbc.8.ip   q2, a1, 0                   // broadcast-load -127 from stack into all lanes of q2
    loopnez a4, .Lsimd_loop                     // repeat loop a4 times (zero-overhead loop counter)
        ee.vmax.s8.ld.incp  q0, a2, q4, q0, q2  // q4 = max(q0, -127); replace -128 with -127, load next 16 bytes into q0, post-increment a2
        ee.vmul.s8 q3, q4, q1                   // q3 = q4 * -1; negates all q4 values
        ee.vmax.s8 q4, q4, q3                   // q4 = max(q4, q3); effectively performs abs(q4)
        ee.vst.128.ip  q4, a3, 16               // store result q4 to [a3], post-increment a3 by 16
    .Lsimd_loop:

    addi a2, a2, -16                            // adjust a2 pointer back to the last processed element  

    .Ltail_start:  
    sext a7, a7, 7                              // sign extends a7 to 32 bits

    // Handle remaining elements that are not a multiple of 16
    loopnez a5, .Ltail_loop
        l8ui a8, a2, 0                          // loads and sign-extends the element into a8
        sext a8, a8, 7 
        
        max a8, a8, a7                          // sets a8 to -127 if it is -128
        abs a8, a8                              // takes the abs of a8
 
        s8i a8, a3, 0                           // store the saturated result in address at a4

        addi.n a2, a2, 1                        // increment pointers 
        addi.n a3, a3, 1 
    .Ltail_loop:  

    movi.n a2, 0                                //return exit code 0 (success)
    retw.n
