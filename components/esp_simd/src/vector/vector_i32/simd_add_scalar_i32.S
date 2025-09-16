.section .text
.global simd_add_scalar_i32
.type simd_add_scalar_i32, @function

/**
 * @brief Performs vec+scalar addtion of an int32_t arr and int32_t scalar value. 
 *
 * This function calculates the addition of a scalar an arr of 32-bit signed integers using the zero-overhead hardware loop.  
 *
 * @param a2 Pointer to the first input arr (int32_t*).
 * @param a3 Pointer to a scalar value (int32_t*)
 * @param a4 Pointer to the ouptut arr (int32_t*).
 * @param a5 Number of elements in the input/output arrs (must be equal for both). 
 *
 * @return 0 on success.
 *  
 * @pre The size in a5 must match the number of elements in each arr.
 * @pre arrs need to be 128 bit aligned
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_add_scalar_i32:
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a14, a5, 0, 2                         // extracts the lowest 2 bits of a5 into a14 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4) 

    // Processes 16 byte chunks 
    ee.vld.128.ip      q0, a2, 16               // loads 16 bytes from a2 into q0 (128-bit vector load), increments a2
    ee.vldbc.32.ip     q1, a3, 0                // load the 32-bit scalar value at the address of a3 and broadcasts it into q1,
    loopnez a5, .Lsimd_end  
        ee.vadds.s32.ld.incp q0, a2, q4, q0, q1 // adds q0 and q1, stores result in q4, increments a2, updates q0
        ee.vst.128.ip     q4, a4, 16            // stores result from q4 into a4, increments a4
    .Lsimd_end:

    // Scalar tail                              // q0 is loaded either from .Lsimd_loop or the load before it 
    ee.vadds.s32      q0, q0, q1                // performs saturated int32 addition on q0 and q1 into q0

    beqz a14, .func_end                         // ends if no scalar tail
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
