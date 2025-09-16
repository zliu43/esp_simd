.section .text
.global simd_mul_shift_i32
.type simd_mul_shift_i32, @function

/**
 * @brief Performs element-wise multiplication, followed by bitshift of two int32_t vectors using SIMD.
 *
 * This function uses 4x loop unrolling to process 128-bit chunks. Any remaining elements are processed using a scalar tail.
 *
 * @param a2 Pointer to the first input vector (int32_t*).
 * @param a3 Pointer to the second input vector (int32_t*).
 * @param a4 Pointer to the output/result vector (int32_t*).
 * @param a5 Bit shift of the output
 * @param a6 Number of elements in the input/output vectors (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vector pointers (a2, a3, a4) must be 128-bit aligned and the number of elements in a5 must be a multiple of 4
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a5 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_mul_shift_i32:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a7, a5, 27, 5                             // if shift_amount >31 return VECTOR_INVALID ARGUMENT
    bnez  a7, .Lbad_shift
    extui a7, a6, 0, 2                          // extracts the lowest 2 bits of a6 into a7 (a6 % 4), for tail processing
    srli a6, a6, 2                              // shift a6 right by 2 to get the number of 16-byte blocks (a6 / 4)
    wsr a5, sar                                 // store the bit shift value in SAR (Shift Amount Register)

    // 4x loop unrolling
    loopnez a6, .Lsimd_loop                     // loop until a6 == 0
        l32i.n a8, a2, 0                        // load a[i] and b[i] into a8, a9 
        l32i.n a9, a3, 0                        // load a[i+1] and b[i+1] into a10, a11
        l32i.n a10, a2, 4
        l32i.n a11, a3, 4

        mulsh a12, a8, a9                       // mulh a8, a9 to a12
        mull a8, a8, a9                         // mull a8, a9 to a8
        src a8, a12, a8                         // shift by SAR
        s32i.n a8, a4, 0                        // store valye

        mulsh a12, a10, a11                     // repeat for a[i+1] and b[i+1]
        mull a8, a10, a11
        src a9, a12, a8
        s32i.n a9, a4, 4                        // store with offset 4

        l32i.n a8, a2, 8                        // repeat for a[i+2] and b[i+2]
        l32i.n a9, a3, 8
        l32i.n a10, a2, 12                      // repeat for a[i+3] and b[i+3]
        l32i.n a11, a3, 12

        mulsh a12, a8, a9 
        mull a8, a8, a9
        src a8, a12, a8
        s32i.n a8, a4, 8

        mulsh a12, a10, a11 
        mull a8, a10, a11
        src a9, a12, a8
        s32i.n a9, a4, 12 

        addi a2, a2, 16
        addi a3, a3, 16
        addi a4, a4, 16
    .Lsimd_loop:

    // Handle remaining elements that are not a multiple of 4
    loopnez a7, .Ltail_loop
        l32i.n a8, a2, 0                         // loads and sign-extends the elements of the two vectors
        l32i.n a9, a3, 0
        mulsh a10, a8, a9
        mull a11, a8, a9
        addi.n a2, a2, 4                         // increment pointers
        addi.n a3, a3, 4
        src a10, a10, a11
        s32i.n a10, a4, 0
        addi.n a4, a4, 4
    .Ltail_loop:

    movi.n a2, 0                                // return VECTOR_SUCCESS
    retw.n

    .Lbad_shift:
    movi.n a2, 2                                // return VECTOR_INVALID_ARGUMENT
    retw.n