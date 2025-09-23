.section .text
.global simd_add_scalar_f32
.type simd_add_scalar_f32, @function

/**
 * @brief Performs vec+scalar addtion of an float32 arr and float32 scalar value. 
 *
 * This function calculates the addition of a scalar an arr of 32-bit float using the zero-overhead hardware loop.  
 *
 * @param a2 Pointer to the first input arr (float32*).
 * @param a3 Pointer to a scalar value (float32*)
 * @param a4 Pointer to the ouptut arr (float32*).
 * @param a5 Number of elements in the input/output arrs (must be equal for both). 
 *
 * @return 0 on success.
 *  
 * @pre The size in a5 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_add_scalar_f32:
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a6, a5, 0, 2                          // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4)

    lsip f4, a3, 0
    beqz a5, .Ltail_start
    
    ee.ldf.128.ip f3, f2, f1, f0, a2, 16        // loads first loop  
    add.s f8, f3, f4                            // precomputes first cycle
    add.s f7, f2, f4 
    add.s f6, f1, f4 
    add.s f5, f0, f4 

    addi a5, a5, -1
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.ldf.128.ip f3, f2, f1, f0, a2, 16    // load 4 elements from vec1, increment a2 
        ee.stf.128.ip f8, f7, f6, f5, a4, 16    // store result
        add.s f8, f3, f4 
        add.s f7, f2, f4 
        add.s f6, f1, f4 
        add.s f5, f0, f4 
    .Lsimd_loop:

    ee.stf.128.ip f8, f7, f6, f5, a4, 16    // store result 

    .Ltail_start:
    loopnez a6, .Ltail_loop
        lsip f0, a2, 4                          // load the first element, increment a2 
        add.s f0, f0, f4                        // performs f32 addition
        ssip f0, a4, 4                          // stores the result, increments a4
    .Ltail_loop:
 
    movi.n a2, 0                                // return exit code 0 (success)
    retw.n