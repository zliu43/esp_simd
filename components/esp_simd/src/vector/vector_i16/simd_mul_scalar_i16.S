.section .text
.global simd_mul_scalar_i16
.type simd_mul_scalar_i16, @function

/*
    * Function: simd_mul_scalar_i16
    * Description: Broadcast multiplies a scalar to a vector of 16 16-bit integers element-wise.
    * This function uses the PIE SIMD instructions to perform the addition in parallel.
    * Arguments:
    *   a2 - Pointer to the first vector (int16_t[16])
    *   a3 - Pointer to a scalar value (int16_t)
    *   a4 - Pointer to the result vector (int16_t[16])
    *   a5 - The shift value
    *   a6 - number of elements in the vector
    * Returns: 0 on success
*/
simd_mul_scalar_i16:
    entry a1, 16                                    // reserve 16 bytes for the stack frame

    extui a7, a5, 28, 4                             // if shift_amount >15 return VECTOR_INVALID ARGUMENT
    bnez  a7, .Lbad_shift

    extui a7, a6, 0, 3                              // moves the last 4 bits (i.e. number of elements that are not a multiple of 16) into a7
    srli a6, a6, 3                                  // bit shift to convert number of 8-bit elements to number of 128-bit blocks
    wsr a5, SAR                                     // set the Shift Amount Register (SAR) to the value in a5
    beqz a6, .Ltail_start                           // if a6 is zero, skip the SIMD addition loop

    // SIMD addition loop for 16 byte blocks
    ee.vld.128.ip     q0, a2, 16                    // loads 16 bytes from a2 into q0 (128-bit vector load), increments a2
    ee.vldbc.16.ip     q1, a3, 0                    // load the 16-bit scalar value at the address of a3 and broadcasts it into q1,
    loopnez a6, .Lsimd_end
        ee.vmul.s16.ld.incp q0, a2, q4, q0, q1      // adds q0 and q1, stores result in q4, increments a2, updates q0
        ee.vst.128.ip     q4, a4, 16                // stores result from q4 into a4, increments a4
    .Lsimd_end:

    addi a2, a2, -16                                // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    .Ltail_start:

    // After processing all full 16-byte blocks, we handle the remaining elements
    l16si a9, a3, 0                                 // Load the scalar value to be added, sign-extended to 32 bit 

    loopnez a7, .Ltail_end
        l16si a8, a2, 0                             // Loads and sign-extends the elements of the scalar tail 

        mull a8, a8, a9                             // perform signed multiplication
        srl a8, a8                                  // apply the bit shift from SAR
        s16i a8, a4, 0                              // store the shifted result in address at a4

        addi a2, a2, 2                              // increment pointers
        addi a4, a4, 2
    .Ltail_end:

    movi.n a2, 0                                    // return VECTOR_SUCCESS
    retw.n

    .Lbad_shift:
    movi.n a2, 2                                    // return VECTOR_INVALID_ARGUMENT
    retw.n
