.section .text
.global simd_compare_eq_i32
.type simd_compare_eq_i32, @function

/**
 * @brief Creates a masking using the element-wise comparison of two int32_t vectors using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently compare two vectors of 32-bit signed integers.
 * At each index, result[i] = arr1[i] == arr2[i] ? -1 : 0;
 * The addition is performed in parallel across 128-bit vector registers, processing 4 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 4) are handled sequentially. 
 *
 * @param a2 Pointer to the first input vector (int32_t*).
 * @param a3 Pointer to the second input vector (int32_t*).
 * @param a4 Pointer to the output/result vector (int32_t*).
 * @param a5 Number of elements in the input/output vectors (must be equal for all three).
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
simd_compare_eq_i32:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a15, a5, 0, 2                         // extracts the lowest 2 bits of a5 into a15 (a5 % 4), for tail processing
    srli a5, a5, 2                              // shift a5 right by 2 to get the number of 16-byte blocks (a5 / 4)
    beqz a5, .Ltail_start                       // if no full blocks (a5 == 0), skip SIMD and go to scalar tail
    
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.vld.128.ip   q0, a2, 16
        ee.vld.128.ip   q1, a3, 16
        ee.vcmp.eq.s32  q2, q0, q1
        ee.vst.128.ip   q2, a4, 16
    .Lsimd_loop:
        
    .Ltail_start:
    // Handle remaining elements that were not part of a full 16-byte block  
    movi a14, -1 
    loopnez a15, .Ltail_loop
        l32i.n a6, a2, 0                         // loads and sign-extends the elements of the two vectors
        l32i.n a7, a3, 0                         
        addi.n a2, a2, 4                        // increment pointers
        addi.n a3, a3, 4
        movi.n a10, 0                           // sets a10 to 0 
        xor a6, a6, a7                          // a7 = a8 - a7
        moveqz a10, a14, a6                     // sets a10 to -1 if a6 == 0 
        s32i.n a10, a4, 0                       // store the saturated result in address at a4
        addi.n a4, a4, 4 
    .Ltail_loop:  

    movi.n a2, 0                                //return exit code 0 (success)
    retw.n
