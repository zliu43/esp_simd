.section .text
.global simd_ceil_f32
.type simd_ceil_f32, @function

/**
 * @brief Performs ceiling on an float arr.
 *
 * Computes C[i] = A[i] < max_val ? A[i] : max_val for each element in the input arrays.
 * Processes data in 16-byte (4 × floats) blocks using a zero-overhead loop
 * for improved throughput, followed by a scalar loop for any remaining elements.
 *
 * @param a2 Pointer to the first input arr (float*).
 * @param a3 Pointer to the ouptut arr (float*). 
 * @param a4 Pointer to the max value (float*) for ceiling
 * @param a5 Number of elements in the input/output arrs (must be equal for all three).   
 *
 * @return 0 on success. 
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_ceil_f32:
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a6, a5, 0, 2                          // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4)
   
    lsip f4, a4, 0                              // loat ceil into f4
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.ldf.128.ip f3, f2, f1, f0, a2, 16    // load 4 elements from vec1, increment a2  
        olt.s   b0, f4, f3                      // b0 = (ceil < cur)
        olt.s   b1, f4, f2           
        olt.s   b2, f4, f1                      
        olt.s   b3, f4, f0        
        movt.s  f3, f4, b0                  
        movt.s  f2, f4, b1                  
        movt.s  f1, f4, b2      
        movt.s  f0, f4, b3
        ee.stf.128.ip f3, f2, f1, f0, a3, 16    // store result
    .Lsimd_loop:
 
    loopnez a6, .Ltail_loop
        lsip f0, a2, 4                          // load the first element, increment a2 
        olt.s   b0, f4, f0                      // b0 = (ceil < cur)
        movt.s  f0, f4, b0
        ssip f0, a3, 4                          // stores the result, increments a3
    .Ltail_loop:
 
    movi.n a2, 0                                // return exit code 0 (success)
    retw.n
