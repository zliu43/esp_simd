#include "vector.h" 


void app_main(){
    /**
     * Initializing vectors:
     * The vector_t struct has four fields:
     *      - void* data        // pointer to the data buffer, *must* be 128-bit aligned
     *      - size_t size       // number of elements the buffer can hold (**not** the number of bytes!)
     *      - dtype type        // currently supported types are DTYPE_INT8, DTYPE_INT16, DTYPE_INT32, DTYPE_FLOAT32
     *      - bool owns_data    // true if the container owns the data, (i.e. made using vector_create)
     * 
     * Vectors can be declared using the vector_create() function or the VECTOR_STACK_INIT macro.
     * If manually declaring the vector, make sure the data is 128 bit aligned. The vector_create()
     * function and VECTOR_STACK_INIT macro provided ensure alignment.  
     */

    // Initializing a vector on the stack
    VECTOR_STACK_INIT(vec1, 512, DTYPE_INT32);

    // Initializing a vector using heap memory:
    vector_t* vec2_ptr = vector_create(512, DTYPE_INT32);   // vector_create returns null when memory allocation fails
    if(vector_ok(vec2_ptr) != VECTOR_SUCCESS){
        // Error handling when system fails to allocate memory
    }

    // Using a vector as a wrapper for existing data
    alignas(16) int32_t data[512];                          // The data buffer **must** be 128-bit aligned
    vector_t vec3 = {
        .data = (void*)data,
        .size = 512,
        .type = DTYPE_INT32,
        .owns_data = false
    };
    assert(vector_ok(&vec3) == VECTOR_SUCCESS);

    /**
     * Destroying vectors:
     * vector_free_data(vector_t* vec) - attempts to free the data buffer if owns_data == true
     * vector_destroy(vector_t* vec)   - frees the data if owns_data == true, and frees the container
     *                              - should mainly be used with vector_create()
     */

    vector_destroy(vec2_ptr); 
}