.section .text
.global simd_abs_f32
.type simd_abs_f32, @function

/**
 * @brief Performs element-wise absolute value of an float. 
 *
 * This function calculates the absolute value an arr of 32-bit float using the zero-overhead hardware loop.  
 *
 * @param a2 Pointer to the first input arr (float*).
 * @param a3 Pointer to the ouptut arr (float*).
 * @param a4 Number of elements in the input/output arrs (must be equal for all three).
 *
 * @return 0 on success.
 *  
 * @pre The size in a4 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_abs_f32:
    entry a1, 16                                    // reserve 16 bytes for the stack frame 
    movi  a6, 0x7fffffff                            // set up bitmask
    s32i.n a6, a1, 0                                // store bitmask on stack for broadcast load
    extui a5, a4, 0, 2                              // extracts the lowest 2 bits of a4 into a5 (a4 % 4), for tail processing
    srli a4, a4, 2                                  // shift a4 right by 2 to get the number of 16-byte blocks a4 = (a4 / 4)

    // Processes 16 byte chunks
    ee.vldbc.32 q0, a1                              // broadcast loads bitmask into vector register       
    loopnez a4, .Lsimd_loop                     
        ee.vld.128.ip q1, a2, 16                    // loads float data
        ee.andq q1, q1, q0                          // applies bitmask, zeroing sign bit
        ee.vst.128.ip q1, a3, 16                    // stores result
    .Lsimd_loop:  

    loopnez a5, .Ltail_loop
        l32i.n a7, a2, 0                            // loads vector data
        and a7, a7, a6                              // applies bitmask
        s32i.n a7, a3, 0                            // store the result in address at a3
        addi a2, a2, 4                              // increment pointers
        addi a3, a3, 4
    .Ltail_loop:

    movi.n a2, 0                                    // return VECTOR_SUCCESS
    retw.n
