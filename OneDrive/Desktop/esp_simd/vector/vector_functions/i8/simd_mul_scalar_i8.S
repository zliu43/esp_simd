.section .text
.global simd_mul_scalar_i8
.type simd_mul_scalar_i8, @function

/*
    * Function: simd_mul_scalar_i8
    * Description: Broadcast multiplies a scalar to a vector of 16 8-bit integers element-wise. 
    * This function uses the PIE SIMD instructions to perform the addition in parallel.
    * Arguments:
    *   a2 - Pointer to the first vector (int8_t[16])
    *   a3 - Pointer to a scalar value (int8_t)
    *   a4 - Pointer to the result vector (int8_t[16])
    *   a5 - The shift value
    *   a6 - number of elements in the vector
    * Returns: 0 on success
*/
simd_mul_scalar_i8:
    entry a1, 16                                    // reserve 16 bytes for the stack frame
    extui a7, a6, 0, 4                              // moves the last 4 bits (i.e. number of elements that are not a multiple of 16) into a7
    srli a6, a6, 4                                  // bit shift to convert number of 8-bit elements to number of 128-bit blocks
    beqz a6, .Ltail_start                           // if a6 is zero, skip the SIMD addition loop

    wsr a5, SAR                                     // set the Shift Amount Register (SAR) to the value in a5
    // SIMD addition loop for 16 byte blocks
    ee.vld.128.ip     q0, a2, 16                    // loads 16 bytes from a2 into q0 (128-bit vector load), increments a2
    ee.vldbc.8.ip     q1, a3, 0                     // load the 8-bit scalar value at the address of a3 and broadcasts it into q1,
    loopnez a6, .Lsimd_end  
        ee.vmul.s8.ld.incp q0, a2, q4, q0, q1      // adds q0 and q1, stores result in q4, increments a2, updates q0
        ee.vst.128.ip     q4, a4, 16                // stores result from q4 into a4, increments a4
    .Lsimd_end:

    addi a2, a2, -16 // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    .Ltail_start:

    // After processing all full 16-byte blocks, we handle the remaining elements 
    l8ui a9, a3, 0 // Load the scalar value to be added, sign-extended to 32 bits
    sext a9, a9, 7

    loopnez a7, .Ltail_end
        l8ui a8, a2, 0 // Loads and sign-extends the elements of the scalar tail 
        sext a8, a8, 7
        
        mull a8, a8, a9 // perform signed multiplication
        srl a8, a8 // apply the bit shift from SAR
        s8i a8, a4, 0 // store the shifted result in address at a4
 
        addi a2, a2, 1 // increment pointers 
        addi a4, a4, 1 
    .Ltail_end:  

    movi.n a2, 0 //return exit code 0 (success)
    retw.n
