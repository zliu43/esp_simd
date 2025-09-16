.section .text
.global simd_ceil_i32
.type simd_ceil_i32, @function

/**
 * @brief Performs ceiling on an int32_t arr using SIMD.
 *
 * Computes C[i] = A[i] < max_val ? A[i] : max_val for each element in the input arrays.
 * Processes data in 16-byte (4 × int32_t) blocks using a zero-overhead loop
 * for improved throughput, followed by a scalar loop for any remaining elements.
 *
 * @param a2 Pointer to the first input arr (int32_t*).
 * @param a3 Pointer to the ouptut arr (int32_t*). 
 * @param a4 Pointer to the max value for ceiling
 * @param a5 Number of elements in the input/output arrs (must be equal for all three).   
 *
 * @return 0 on success. 
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_ceil_i32:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 2                          // a6 = a5 % 2 (remaining tail elements)
    srli a5, a5, 2                              // a5 = a5 / 2 (number of 8-byte blocks)   
    beqz a5, .Ltail_start

    ee.vld.128.ip           q0, a2, 16          // load 16 bytes from [a2] into q0, post-increment a2 by 16 
    ee.vldbc.32.ip          q1, a4, 0           // broadcast-load a4 into all lanes of q1
    loopnez a5, .Lsimd_loop                     // repeat loop a5 times (zero-overhead loop counter)
        ee.vmin.s32.ld.incp q0, a2, q2, q0, q1  // q2 = min(q0, q1); applies the ceiling to q0, load next 16 bytes into q0, post-increment a2 
        ee.vst.128.ip       q2, a3, 16          // store result q2 to [a3], post-increment a3 by 16
    .Lsimd_loop:

    addi a2, a2, -16                            // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    .Ltail_start:
    l32i.n a4, a4, 0                            // loads max_val into a4 
    // Handle remaining elements that are not in 16 byte blocks
    loopnez a6, .Ltail_loop
        l32i.n a7, a2, 0                        // loads and sign-extends the element into a7 
        addi.n a2, a2, 4                        // increment pointers 
        min a7, a7, a4                          // applies the ceiling to a7
        s32i.n a7, a3, 0                        // store the result in address at a3 
        addi.n a3, a3, 4 
    .Ltail_loop:  

    movi.n a2, 0                                // return exit code 0 (success)
    retw.n
