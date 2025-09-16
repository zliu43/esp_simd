.section .text
.global simd_add_f32
.type simd_add_f32, @function

/**
 * @brief Performs element-wise addition of two float arrs.  
 *
 * This function adds two arrs of 32-bit floats using 128-bit databus and the zero-overhead hardware loop. 
 *
 * @param a2 Pointer to the first input arr (float*).
 * @param a3 Pointer to the second input arr (float*).
 * @param a4 Pointer to the output/result arr (float*).
 * @param a5 Number of elements in the input/output arr (must be equal for all three).
 *
 * @return 0 on success.  
 * @pre The size in a5 must match the number of elements in each vector.
 * @pre arr points must be 128-bit aligned
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_add_f32:
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a6, a5, 0, 2                          // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4)
   
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.ldf.128.ip f3, f2, f1, f0, a2, 16    // load 4 elements from vec1, increment a2
        ee.ldf.128.ip f7, f6, f5, f4, a3, 16    // load 4 elements from vec2, increment a3
        add.s f3, f3, f7 
        add.s f2, f2, f6 
        add.s f1, f1, f5 
        add.s f0, f0, f4 
        ee.stf.128.ip f3, f2, f1, f0, a4, 16    // store result
    .Lsimd_loop:
 
    loopnez a6, .Ltail_loop
        lsip f0, a2, 4                          // load the first element, increment a2
        lsip f1, a3, 4                          // load the second element, increment a3
        add.s f0, f0, f1                        // performs f32 addition
        ssip f0, a4, 4                          // stores the result, increments a4
    .Ltail_loop:
 
    movi.n a2, 0                                // return exit code 0 (success)
    retw.n
