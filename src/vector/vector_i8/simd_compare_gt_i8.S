.section .text
.global simd_compare_gt_i8
.type simd_compare_gt_i8, @function

/**
 * @brief Creates a masking using the element-wise greater than comparison of two int8_t vectors using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently compare two vectors of 8-bit signed integers.
 * At each index, result[i] = arr1[i] > arr2[i] ? -1 : 0;
 * The addition is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input vector (int8_t*).
 * @param a3 Pointer to the second input vector (int8_t*).
 * @param a4 Pointer to the output/result vector (int8_t*).
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
simd_compare_gt_i8:
    entry a1, 16 // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 4 // extracts the lowest 4 bits of a5 into a6 (a5 % 16), for tail processing
    srli a5, a5, 4 // shift a5 right by 4 to get the number of 16-byte blocks (a5 / 16) 

    // SIMD addition loop for 16-byte blocks
    loopnez a5, .Lsimd_loop                     // loop until a5 == 0
        ee.vld.128.ip     q0, a2, 16            // loads 16 bytes from a2 into q0, then increment a2 by 16
        ee.vld.128.ip     q1, a3, 16            // loads 16 bytes from a3 into q1, increment a3 by 16
        ee.vcmp.gt.s8     q2, q0, q1            // compares q0 and q1, stores mask in q2
        ee.vst.128.ip     q2, a4, 16            // stores 16 bytes from q2 to address at a4, increment a4 by 16
    .Lsimd_loop:
  
    // Handle remaining elements that were not part of a full 16-byte block 
    movi a9, -1 
    loopnez a6, .Ltail_loop
        l8ui a7, a2, 0                          // loads and sign-extends the elements of the two vectors
        l8ui a8, a3, 0 
        sext a7, a7, 7
        sext a8, a8, 7
        
        movi.n a10, 0                           // sets a10 to 0
        
        sub a7, a8, a7                          // a7 = a8 - a7
        movltz a10, a9, a7                      // sets a10 to -1 if a7 < 0


        s8i a10, a4, 0                          // store the saturated result in address at a4

        addi.n a2, a2, 1                        // increment pointers
        addi.n a3, a3, 1
        addi.n a4, a4, 1 
    .Ltail_loop:  

    movi.n a2, 0 //return exit code 0 (success)
    retw.n
