.section .text
.global simd_mul_scalar_i32
.type simd_mul_scalar_i32, @function

/*
    * Function: simd_mul_scalar_i32
    * Description: Broadcast multiplies a scalar to a vector of 4 32-bit integers element-wise. 
    * Arguments:
    *   a2 - Pointer to the first vector (int32_t*)
    *   a3 - Pointer to a scalar value (int32_t)
    *   a4 - Pointer to the result vector (int32_t*)
    *   a5 - The shift value
    *   a6 - number of elements in the vector
    * Returns: 0 on success
*/
simd_mul_scalar_i32:
    entry a1, 16                                    // reserve 16 bytes for the stack frame

    extui a7, a5, 27, 5                             // if shift_amount >31 return VECTOR_INVALID ARGUMENT
    bnez  a7, .Lbad_shift

    extui a7, a6, 0, 2                              // moves the last 2 bits (i.e. number of elements that are not a multiple of 16)  
    srli a6, a6, 2                                  // bit shift to convert number of 32-bit elements to number of 128-bit blocks
    
    
    l32i a8, a3, 0                                  // Load scalar value
    wsr a5, sar                                      // Load shift amount register
    loopnez a6, .Lhead_end                          // 4x loop unrolling
        l32i.n   a9,  a2, 0                         // Load first two values
        l32i.n   a10, a2, 4 
        mulsh     a11, a9,  a8                       // multiply into int64
        mull     a9,  a9,  a8
        mulsh     a12, a10, a8           
        mull     a10, a10, a8
        src      a9, a11, a9                        // right funnel shift (a11||a9) >> SAR 
        src      a10, a12, a10
        s32i.n      a9,  a4, 0
        s32i.n      a10, a4, 4  
        l32i.n   a9,  a2, 8
        l32i.n   a10, a2, 12 
        mulsh     a11, a9,  a8
        mull     a9,  a9,  a8 
        mulsh     a12, a10, a8
        mull     a10, a10, a8
        src      a9,  a11, a9
        src      a10, a12, a10 
        s32i.n      a9,  a4, 8
        s32i.n      a10, a4, 12 
        addi     a2,  a2, 16                        // increment pointers
        addi     a4,  a4, 16
    .Lhead_end:
  
    // After processing all full 16-byte blocks, we handle the remaining elements 
    loopnez a7, .Ltail_end 
        l32i.n a9, a2, 0
        mull a10, a9, a8
        mulsh a11, a9, a8
        addi.n  a2, a2, 4                              
        src a9, a11, a10
        s32i.n a9, a4, 0 
        addi.n  a4, a4, 4
    .Ltail_end:

    movi.n a2, 0                                    // return VECTOR_SUCCESS
    retw.n

    .Lbad_shift:
    movi.n a2, 2                                    // return VECTOR_INVALID_ARGUMENT
    retw.n