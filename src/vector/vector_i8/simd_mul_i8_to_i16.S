.section .text
.global simd_mul_i8_to_i16
.type simd_mul_i8_to_i16, @function

/**
 * @brief Performs element-wise multiplication of two int8_t vectors, producing int16_t results using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently multiply two vectors of 8-bit signed integers.
 * Each multiplication produces a 16-bit signed integer result, stored in a separate int16_t output vector.
 * The operation is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially.
 *
 * @param a2 Pointer to the first input vector (int8_t*).
 * @param a3 Pointer to the second input vector (int8_t*).
 * @param a4 Pointer to the output/result vector (int16_t*). 
 * @param a5 Number of elements in the input vectors and output vector.
 *
 * @return 0 on success.
 *  
 * @note Input vectors (a2, a3) must be 128-bit aligned. The output vector (a4) must be 128-bit aligned for best performance.
 *       The number of elements in a5 should be a multiple of 16 for full SIMD efficiency.
 *       Remaining elements are processed using scalar operations.
 *
 * @pre All input and output pointers must be non-null and properly aligned.
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_mul_i8_to_i16:
    entry a1, 16 // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 4 // extracts the lowest 4 bits of a5 into a6 (a5 % 16), for tail processing
    srli a5, a5, 4 // shift a5 right by 4 to get the number of 16-byte blocks (a5 / 16)
    beqz a5, .Ltail_start // if no full blocks (a5 == 0), skip SIMD and go to scalar tail  

    // SIMD multiplication loop for 16-byte blocks
    ee.vld.128.ip     q1, a3, 16                // loads 16 bytes from a3 into q1, increment a3 by 16
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.vld.128.ip     q0, a2, 16            // loads 16 bytes from a2 into q0, then increment a2 by 16
        ssai 8                                  // store the bit shift value in SAR (Shift Amount Register) (high 8 bits for multiplication)
        ee.vmul.s8  q2, q0, q1                  // multiplies q0 and q1, stores result in q2 
        ssai 0                                  // store the bit shift value in SAR (Shift Amount Register) (low 8 bits for multiplication)
        ee.vmul.s8.ld.incp q1, a3, q3, q0, q1   // multiplies q0 and q1, stores result in q3, incr  
        ee.vzip.8 q3, q2                        // zips the two vectors q2 and q3 into a single vector, interleaving elements
        ee.vst.128.ip     q3, a4, 16            // stores first half of the zipped vector to address at a4, increment a4 by 16
        ee.vst.128.ip     q2, a4, 16            // stores second half of the zipped vector to address at a4, increment a4 by 16
    .Lsimd_loop:

    // Adjust pointers back to the last processed element, (they go too far due to the last increment in the loop)
    // l8ui does not support negative offsets, so we need to adjust the pointers back 
    addi a3, a3, -16 // adjust a3 pointer back to the last processed element 
    
    .Ltail_start: 
    // Handle remaining elements that are not a multiple of 16
    loopnez a6, .Ltail_loop
        l8ui a7, a2, 0 // loads and sign-extends the elements of the two vectors
        l8ui a8, a3, 0 
        sext a7, a7, 7
        sext a8, a8, 7

        mull a7, a7, a8 // perform signed multiplication 
        s16i a7, a4, 0 // store the shifted result in address at a4

        addi a2, a2, 1 // increment pointers
        addi a3, a3, 1
        addi a4, a4, 2 
    .Ltail_loop:  

    movi.n a2, 0 //return exit code 0 (success)
    retw.n
