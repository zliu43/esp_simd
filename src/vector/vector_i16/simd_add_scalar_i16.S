.section .text
.global simd_add_scalar_i16
.type simd_add_scalar_i16, @function

/*
    * Function: simd_add_scalar_i16
    * Description: Broadcast adds a scalar to a vector of 16 16-bit integers element-wise. int_16 addition is saturated.
    * This function uses the PIE SIMD instructions to perform the addition in parallel.
    * Arguments:
    *   a2 - Pointer to the first vector (int16_t*)
    *   a3 - Pointer to a scalar value (int16_t*)
    *   a4 - Pointer to the result vector (int16_t*)
    *   a5 - number of elements in the vector
    * Returns: 0 on success
*/
simd_add_scalar_i16:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 3                          // moves the last 3 bits (i.e. number of elements that are not a multiple of 16) into a6
    srli a5, a5, 3                              // bit shift to convert number of 16-bit elements to number of 128-bit blocks
    beqz a5, .Ltail_start                       // if a5 is zero, skip the SIMD addition loop

    // SIMD addition loop for 16 byte blocks
    ee.vld.128.ip     q0, a2, 16                // loads 16 bytes from a2 into q0 (128-bit vector load), increments a2
    ee.vldbc.16.ip     q1, a3, 0                // load the 16-bit scalar value at the address of a3 and broadcasts it into q1,
    loopnez a5, .Lsimd_end  
        ee.vadds.s16.ld.incp q0, a2, q4, q0, q1 // adds q0 and q1, stores result in q4, increments a2, updates q0
        ee.vst.128.ip     q4, a4, 16            // stores result from q4 into a4, increments a4
    .Lsimd_end:

    addi a2, a2, -16                            // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    .Ltail_start:

    // After processing all full 16-byte blocks, we handle the remaining elements
    movi a9, 32767                              // Load saturation limit for positive values
    movi a10, -32768                            // Load saturation limit for negative values

    l16si a8, a3, 0                             // Load the scalar value to be added, sign-extended to 32 bits 
    loopnez a6, .Ltail_end
        l16si a7, a2, 0                         // Loads and sign-extends the elements of the two vectors 
        addi a2, a2, 2                          // increment pointers 
        add a7, a7, a8                          // Adds the two signed 16-bit integers 

        // Saturation logic
        salt a11, a7, a10                       // sets a11 to 1 if a7 is less than -32768
        movnez a7, a10, a11                     // Conditional move if non-zero sets a7 to -32768 if a11 is non-zero
        salt a11, a9, a7                        // sets a11 to 1 if a7 is greater than 32767
        movnez a7, a9, a11                      // Conditional move if non-zero sets a7 to 32767 if a11 is non-zero

        s16i a7, a4, 0                          // store the result in the result vector 
        addi a4, a4, 2                          // increment pointers
    .Ltail_end:  

    movi.n a2, 0                                // return VECTOR_SUCCESS
    retw.n
