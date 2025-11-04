.section .text
.global simd_i8_to_i16
.type simd_i8_to_i16, @function

/**
 * @brief Performs a copy + widen operation on an int8_t vector to an int16_t vector using SIMD.
 *
 * This function uses PIE SIMD instructions to efficiently widen a vector of 8-bit signed integers.
 * The operation is performed in parallel across 128-bit vector registers, processing 16 elements per loop iteration.
 * Any remaining elements (if the length is not a multiple of 16) are handled sequentially. 
 *
 * @param a2 Pointer to the first input vector (int8_t*). 
 * @param a3 Pointer to the output/result vector (int16_t*).
 * @param a4 Number of elements in the input/output vectors (must be equal for all three).
 *
 * @return 0 on success.
 *
 * @note All vector pointers (a2, a3) must be 128-bit aligned and the number of elements in a4 must be a multiple of 16 for full SIMD processing. Non-multiple tail elements are handled separately with scalar operations.
 *
 * @pre All input and output pointers must be non-null and 128-bit aligned.
 * @pre The size in a4 must match the number of elements in each vector.
 *
 * @warning Misaligned data or incorrect element count may result in undefined behavior or hardware exceptions.
 */
simd_i8_to_i16:
    entry a1, 16                // reserve 16 bytes for the stack frame
    extui a5, a4, 0, 4          // extracts the lowest 4 bits of a4 into a5 (a4 % 16), for tail processing
    srli a4, a4, 4              // shift a4 right by 4 to get the number of 16-byte blocks (a4 / 16)
    beqz a4, .Ltail_start       // if no full blocks (a4 == 0), skip SIMD and go to scalar tail

    // Prepare constant for sign extension
    movi.n a6, 0x80             // load 0x80 into a6 for sign extension
    s32i a6, a1, 0              // store 0x80 into stack frame for broadcast loading


    /**
        SIMD Widening Logic:
        We use SIMD operations to perform the following function.
        int16_t* output = (int16_t*)((int8_t*)input_vector + 0x80) - 0x80; 
        This effectively sign-extends each int8_t to int16_t by first offsetting the values to make them non-negative, then widening, and finally reapplying the offset.
    */

    // SIMD addition loop for 16-byte blocks 
    ee.vldbc.8    q2, a1        // broadcast loads 0x80 bytes from a1 into q2 as int8_ts
    ee.vldbc.16   q3, a1        // broadcast loads 0x80 bytes from a1 into q3 as int16_ts
    loopnez a4, .Lsimd_loop                     // loop until a4 == 0
        ee.vld.128.ip     q0, a2, 16            // loads 16 bytes from a2 into q0, increment a3 by 16 
        ee.xorq           q1, q1, q1            // q1 = 0x00        (clear q1)
        ee.xorq           q0, q0, q2            // q0 = q0 ^ 0x80   (to offset for sign-extension)
        ee.vzip.8         q0, q1                // interleave bytes to widen
        ee.vsubs.s16       q0, q0, q3           // q0 = q0 - 0x80   (complete sign-extension to int16_t)
        ee.vsubs.s16       q1, q1, q3           // q1 = q1 - 0x80   (complete sign-extension to int16_t)
        ee.vst.128.ip     q0, a3, 16            // store the result from q0 into a3, increment a3 by 16
        ee.vst.128.ip     q1, a3, 16            // store the result from q1 into a3, increment a3 by 16
    .Lsimd_loop: 

    .Ltail_start: 
    // Handle remaining elements that are not a multiple of 16
    loopnez a5, .Ltail_loop
        l8ui a7, a2, 0          // loads and sign-extends the elements of the two vectors 
        sext a7, a7, 7          // sign-extend the int8_t to int16_t 
         
        s16i a7, a3, 0          // store the extended result in address at a3

        addi.n a2, a2, 1        // increment pointers
        addi.n a3, a3, 2 
    .Ltail_loop:  

    movi.n a2, 0                // return VECTOR_SUCCESS
    retw.n
