.section .text
.global simd_min_f32
.type simd_min_f32, @function

/**
 * @brief Takes the element-wise min of two arrays and saves the result to output.
 *
 * Computes C[i] = A[i] < B[i] ? A[i] : B[i] for each element in the input arrays.
 * Processes data in 16-byte (4 × float) blocks using a zero-overhead loop
 * for improved throughput, followed by a scalar loop for any remaining elements.
 *
 * @param a2 Pointer to the first input arr (float*).
 * @param a3 Pointer to the second input arr (float*). 
 * @param a4 Pointer to the output arr (float*) 
 * @param a5 Number of elements in the input/output arrs (must be equal for all three).   
 *
 * @return 0 on success. 
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_min_f32:
    entry a1, 16                                // reserve 16 bytes for the stack frame 
    extui a6, a5, 0, 2                          // extracts the lowest 2 bits of a5 into a6 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks a5 = (a5 / 4)
    
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.ldf.128.ip f3, f2, f1, f0, a2, 16    // load 4 elements from vec1, increment a2  
        ee.ldf.128.ip f7, f6, f5, f4, a3, 16    // load 4 elements from vec1, increment a2  
        olt.s   b0, f7, f3                      // b0 = (floor > cur)
        olt.s   b1, f6, f2           
        olt.s   b2, f5, f1                      
        olt.s   b3, f4, f0        
        movt.s  f3, f7, b0                  
        movt.s  f2, f6, b1                  
        movt.s  f1, f5, b2      
        movt.s  f0, f4, b3
        ee.stf.128.ip f3, f2, f1, f0, a4, 16    // store result
    .Lsimd_loop:
 
    loopnez a6, .Ltail_loop
        lsip f0, a2, 4                          // load the first element, increment a2 
        lsip f4, a3, 4                          // load the first element, increment a2 
        olt.s   b0, f4, f0                      // b0 = (floor > cur)
        movt.s  f0, f4, b0
        ssip f0, a4, 4                          // stores the result, increments a4
    .Ltail_loop:
 
    movi.n a2, 0                                // return VECTOR_SUCCESS
    retw.n
