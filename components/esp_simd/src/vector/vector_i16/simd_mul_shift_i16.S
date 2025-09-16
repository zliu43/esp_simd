.section .text
.global simd_mul_shift_i16
.type simd_mul_shift_i16, @function

/**
 * @brief Performs element-wise multiplication, followed by bitshift of two int16_t vectors using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently multiply two vectors of 16-bit signed integers.
 * The multiplication is performed in parallel across 128-bit vector registers, processing 8 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 8) are handled sequentially.
 * The result is stored in a third vector, with an optional bit shift applied to the final result.
 *
 * @param a2 Pointer to the first input vector (int16_t*).
 * @param a3 Pointer to the second input vector (int16_t*).
 * @param a4 Pointer to the output/result vector (int16_t*).
 * @param a5 Bit shift of the output
 * @param a6 Number of elements in the input/output vectors (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vector pointers (a2, a3, a4) must be 128-bit aligned and the number of elements in a5 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_mul_shift_i16:
    entry a1, 16                                // reserve 16 bytes for the stack  
    extui a7, a5, 28, 4                         // if shift_amount >15 return VECTOR_INVALID ARGUMENT
    bnez  a7, .Lbad_shift
    extui a7, a6, 0, 3                          // extracts the lowest 3 bits of a6 into a7 (a6 % 8), for tail processing
    srli a6, a6, 3                              // shift a6 right by 3 to get the number of 16-byte blocks (a6 / 8)
    wsr a5, sar                                 // store the bit shift value in SAR (Shift Amount Register)
    beqz a6, .Ltail_start                       // if no full blocks (a6 == 0), skip SIMD and go to scalar tail

    // SIMD multiplication loop for 16-byte blocks
    ee.vld.128.ip     q0, a2, 16                // loads 16 bytes from a2 into q0, then increment a2 by 16
    loopnez a6, .Lsimd_loop                     // loop until a6 == 0
        ee.vld.128.ip     q1, a3, 16            // loads 16 bytes from a3 into q1, increment a3 by 16
        ee.vmul.s16.ld.incp q0, a2, q4, q0, q1  // multiplies q0 and q1, stores result in q4, increments a2, updates q0
        ee.vst.128.ip     q4, a4, 16            // stores 16 bytes from q4 to address at a4, increment a4 by 16
    .Lsimd_loop:

    addi a2, a2, -16                            // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    .Ltail_start: 
    loopnez a7, .Ltail_loop                     // Handle remaining elements that were not part of a full 16-byte block 
        l16si a8, a2, 0                         // loads and sign-extends the elements of the two vectors
        l16si a9, a3, 0 

        mull a8, a8, a9                         // perform signed multiplication
        srl a8, a8                              // apply the bit shift from SAR
        s16i a8, a4, 0                          // store the shifted result in address at a4

        addi a2, a2, 2                          // increment pointers
        addi a3, a3, 2
        addi a4, a4, 2
    .Ltail_loop:

    movi.n a2, 0                                // return VECTOR_SUCCESS
    retw.n

    .Lbad_shift:
    movi.n a2, 2                                // return VECTOR_INVALID_ARGUMENT
    retw.n
