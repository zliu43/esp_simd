.section .text
.global simd_add_scalar_i8
.type simd_add_scalar_i8, @function

/**
 * @brief Performs saturated addition an int8_t arr and a scalar using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently add a scalar to an arr of 8-bit signed integers.
 * The addition is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially.
 * Saturation is applied such that results exceeding the int8_t bounds are clamped to [-128, 127].
 *
 * @param a2 Pointer to the first input vector (int8_t*).
 * @param a3 Pointer to the scalar value (int8_t*).
 * @param a4 Pointer to the output/result vector (int8_t*).
 * @param a5 Number of elements in the input/output vectors (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vector pointers (a2, a4) must be 128-bit aligned and the number of elements in a5 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_add_scalar_i8:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 4                          // moves the last 4 bits (i.e. number of elements that are not a multiple of 16) into a6
    srli a5, a5, 4                              // bit shift to convert number of 8-bit elements to number of 128-bit blocks
    beqz a5, .Ltail_start                       // if a5 is zero, skip the SIMD addition loop

    // SIMD addition loop for 16 byte blocks
    ee.vld.128.ip     q0, a2, 16                // loads 16 bytes from a2 into q0 (128-bit vector load), increments a2
    ee.vldbc.8.ip     q1, a3, 0                 // load the 8-bit scalar value at the address of a3 and broadcasts it into q1,
    loopnez a5, .Lsimd_end  
        ee.vadds.s8.ld.incp q0, a2, q4, q0, q1  // adds q0 and q1, stores result in q4, increments a2, updates q0
        ee.vst.128.ip     q4, a4, 16            // stores result from q4 into a4, increments a4
    .Lsimd_end:

    addi a2, a2, -16                            // adjust a2 pointer back to the last processed element 

    .Ltail_start:

    // After processing all full 16-byte blocks, we handle the remaining elements
    movi.n a9, 127                              // Load saturation limit for positive values
    movi a10, -128                              // Load saturation limit for negative values  

    l8ui a8, a3, 0                              // Load the scalar value to be added, sign-extended to 32 bits
    sext a8, a8, 7

    loopnez a6, .Ltail_end
        l8ui a7, a2, 0                          // Loads and sign-extends the elements of the two vectors
        addi a2, a2, 1                          // increment pointers 
        sext a7, a7, 7
        
        add a7, a7, a8                          // Adds the two signed 8-bit integers

        // Saturation logic
        salt a11, a7, a10                       // sets a11 to 1 if a7 is less than -128
        movnez a7, a10, a11                     // Conditional move if non-zero sets a7 to -128 if a11 is non-zero
        salt a11, a9, a7                        // sets a11 to 1 if a7 is greater than 127 
        movnez a7, a9, a11                      // Conditional move if non-zero sets a7 to 127 if a11 is non-zero
        s8i a7, a4, 0                           // store the result in the result vector

        addi a4, a4, 1                          // increment pointers 
    .Ltail_end:  

    movi.n a2, 0                                // return VECTOR_SUCCESS
    retw.n
