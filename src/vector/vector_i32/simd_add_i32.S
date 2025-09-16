.section .text
.global simd_add_i32
.type simd_add_i32, @function

/**
 * @brief Performs element-wise addition of two int32_t arrs.  
 *
 * This function adds two arrs of 32-bit signed integers using the zero-overhead hardware loop. 
 *
 * @param a2 Pointer to the first input arr (int32_t*).
 * @param a3 Pointer to the second input arr (int32_t*).
 * @param a4 Pointer to the output/result arr (int32_t*).
 * @param a5 Number of elements in the input/output arr (must be equal for all three).
 *
 * @return 0 on success.  
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_add_i32:
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a14, a5, 0, 2                         // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4) 
 
    // SIMD addition loop for 16-byte blocks
    ee.vld.128.ip     q0, a2, 16                // loads 16 bytes from a2 into q0, then increment a2 by 16
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.vld.128.ip     q1, a3, 16            // loads 16 bytes from a3 into q1, increment a3 by 16
        ee.vadds.s32.ld.incp q0, a2, q4, q0, q1 // adds q0 and q1, stores result in q4, increments a2, updates q0
        ee.vst.128.ip     q4, a4, 16            // stores 16 bytes from q4 to address at a4, increment a4 by 16
    .Lsimd_loop:
 
    // Scalar tail                              // q0 is loaded either from .Lsimd_loop or the load before it
    ee.vld.128.ip     q1, a3, 0                 // loads 16 bytes from a3 into q1
    ee.vadds.s32      q0, q0, q1                // performs saturated int32 addition on q0 and q1 into q0

    beqz a14, .func_end                          // ends if no scalar tail
    ee.movi.32.a q0, a6, 0                      
    s32i.n a6, a4, 0 
    addi a14, a14, -1

    beqz a14, .func_end
    ee.movi.32.a q0, a6, 1
    s32i.n a6, a4, 4 
    addi a14, a14, -1

    beqz a14, .func_end
    ee.movi.32.a q0, a6, 2
    s32i.n a6, a4, 8 
    addi a14, a14, -1
     
    .func_end:
    movi.n a2, 0                                // return exit code 0 (success)
    retw.n
