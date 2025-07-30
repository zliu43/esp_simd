.section .text
.global simd_floor_i8
.type simd_floor_i8, @function

/**
 * @brief Performs floor on an int8_t arr using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently floor an arr of 8-bit signed integers.
 * The operation is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input arr (int8_t*).
 * @param a3 Pointer to the ouptut arr (int8_t*). 
 * @param a4 Pointer to the min value for floor
 * @param a5 Number of elements in the input/output arrs (must be equal for all three).   
 *
 * @return 0 on success.
 *
 * @note All arr pointers (a2, a3) must be 128-bit aligned and the number of elements in a5 must be a multiple of 16
 *       for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each arr.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_floor_i8:
    entry a1, 16                                // reserve 16 bytes for the stack frame
    extui a6, a5, 0, 4                          // a6 = a5 % 16 (remaining tail elements)
    srli a5, a5, 4                              // a5 = a5 / 16 (number of 16-byte SIMD blocks) 
    beqz a5, .Ltail_start                       // if no full blocks, skip SIMD loop

    // SIMD loop for 16-byte blocks  
    ee.vld.128.ip   q0, a2, 16                  // load 16 bytes from [a2] into q0, post-increment a2 by 16 
    ee.vldbc.8.ip   q1, a4, 0                   // broadcast-load a4 into all lanes of q1
    loopnez a5, .Lsimd_loop                     // repeat loop a5 times (zero-overhead loop counter)
        ee.vmax.s8.ld.incp  q0, a2, q2, q0, q1  // q2 = min(q0, q1); applies the ceiling to q0, load next 16 bytes into q0, post-increment a2 
        ee.vst.128.ip  q2, a3, 16               // store result q2 to [a3], post-increment a3 by 16
    .Lsimd_loop:

    addi a2, a2, -16 // adjust a2 pointer back to the last processed element (it goes too far due to the last increment in the loop)

    .Ltail_start: 

    l8ui a7, a4, 0                              // loads the floor into a7
    sext a7, a7, 7

    // Handle remaining elements that are not a multiple of 16
    loopnez a6, .Ltail_loop
        l8ui a8, a2, 0                          // loads and sign-extends the element into a8
        sext a8, a8, 7
        
        max a8, a8, a7                          // applies the floor to a8
 
        s8i a8, a3, 0                           // store the result in address at a3

        addi.n a2, a2, 1                        // increment pointers 
        addi.n a3, a3, 1 
    .Ltail_loop:  

    movi.n a2, 0                                //return exit code 0 (success)
    retw.n
