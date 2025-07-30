.section .text
.global simd_relu_i8
.type simd_relu_i8, @function

/**
 * @brief Performs ReLU on an int8_t arr using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently apply ReLU on an arr of 8-bit signed integers.
 * Elements less than zero are multiplied by the provided multiplier and then right-shifted by the shift amount.
 * The operation is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input arr (int8_t*).
 * @param a3 Multiplier
 * @param a4 Shift amount
 * @param a5 Pointer to the output arr (int8_t*)
 * @param a6 Number of elements in the arr
 *
 * @return 0 on success.
 *
 * @note All arr pointers (a2, a5) must be 128-bit aligned and the number of elements in a6 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_relu_i8:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a7, a6, 0, 4                          // a7 = a6 % 16 (remaining tail elements)
    srli a6, a6, 4                              // a6 = a6 / 16 (number of 16-byte SIMD blocks) 
    beqz a6, .Ltail_start                       // if no full blocks, skip SIMD loop

    // SIMD loop for 16-byte blocks   
    loopnez a6, .Lsimd_loop                     // repeat loop a5 times (zero-overhead loop counter)
        ee.vld.128.ip   q0, a2, 16              // load 16 bytes from [a2] into q0, post-increment a2 by 16  
        ee.vrelu.s8    q0, a3, a4               // For each signed byte in q0: if < 0, multiply by lower 16 bits of a3, 
                                                // then arithmetic right shift by lower 6 bits of a4
        ee.vst.128.ip  q0, a5, 16               // store result q2 to [a3], post-increment a3 by 16
    .Lsimd_loop:
 
    .Ltail_start: 
 
    extui a4, a4, 0, 5                          // extracts lower bits in a4
    wsr a4, SAR                                 // sets the Shift Amount Register 

    // Handle remaining elements that are not a multiple of 16
    loopnez a7, .Ltail_loop
        l8ui a8, a2, 0                          // loads and sign-extends the element into a8
        sext a8, a8, 7
        
        mull a9, a8, a3                         // multiplies the multiplier
        sra a9, a9                              // shift by the shift amount
        movltz a8, a9, a8                       // sets a8 as the RELU'd result if a8 is < 0

        s8i a8, a5, 0                           // store the result in address at a5

        addi.n a2, a2, 1                        // increment pointers 
        addi.n a5, a5, 1 
    .Ltail_loop:  

    movi.n a2, 0                                //return exit code 0 (success)
    retw.n
