.section .text
.global simd_add_i16
.type simd_add_i16, @function

/**
 * @brief Performs element-wise saturated addition of two int16_t arrs using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently add two arrs of 16-bit signed integers.
 * The addition is performed in parallel across 128-bit vector registers, processing 8 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 8) are handled sequentially.
 * Saturation is applied such that results exceeding the int16_t bounds are clamped to [-32768, 32767].
 *
 * @param a2 Pointer to the first input arr (int16_t*).
 * @param a3 Pointer to the second input arr (int16_t*).
 * @param a4 Pointer to the output/result arr (int16_t*).
 * @param a5 Number of elements in the input/output arr (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vetor pointers (a2, a3, a4) must be 128-bit aligned and the number of elements in a5 must be a multiple of 8
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_add_i16:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 3                          // extracts the lowest 3 bits of a5 into a6 (a5 % 8), for tail processing
    srli a5, a5, 3                              // shift a5 right by 3 to get the number of 16-byte blocks (a5 / 3)
    beqz a5, .Ltail_start                       // if no full blocks (a5 == 0), skip SIMD and go to scalar tail

    // SIMD addition loop for 16-byte blocks
    ee.vld.128.ip     q0, a2, 16 // loads 16 bytes from a2 into q0, then increment a2 by 16
    loopnez a5, .Lsimd_loop  // loop until a5 == 0
        ee.vld.128.ip     q1, a3, 16 // loads 16 bytes from a3 into q1, increment a3 by 16
        ee.vadds.s16.ld.incp q0, a2, q4, q0, q1 //adds q0 and q1, stores result in q4, increments a2, updates q0
        ee.vst.128.ip     q4, a4, 16 // stores 16 bytes from q4 to address at a4, increment a4 by 16
    .Lsimd_loop:

    addi a2, a2, -16 // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    .Ltail_start:
    // Handle remaining elements that were not part of a full 16-byte block
    movi a9, 32767 // loads upper saturation limit int8_t
    movi a10, -32768 // loads lower saturation limit int8_t

    // Handle remaining elements that are not a multiple of 16
    loopnez a6, .Ltail_loop
        l16si a7, a2, 0 // loads and sign-extends the elements of the two vectors
        l16si a8, a3, 0  
        
        add a7, a7, a8 // perform signed addition

        // Saturation logic
        salt a11, a7, a10 // sets a11 to 1 if a7 is less than lower limit
        movnez a7, a10, a11 // conditional move if non-zero sets a7 to lower limit if a11 is non-zero
        salt a11, a9, a7 // sets a11 to 1 if a7 is greater than upper limit
        movnez a7, a9, a11 // conditional move if non-zero sets a7 to upper limit if a11 is non-zero

        s16i a7, a4, 0 // store the saturated result in address at a4

        addi.n a2, a2, 2 // increment pointers
        addi.n a3, a3, 2
        addi.n a4, a4, 2 
    .Ltail_loop:  

    movi.n a2, 0 //return exit code 0 (success)
    retw.n
