#include "vector.h" 

void app_main(){
    /**
     * the vector struct stores the data buffer as a void*. It must be cast to the appropriate type before indexing.
     */
    VECTOR_STACK_INIT(vec1, 8, DTYPE_INT32);
    int32_t* vec1_data = (int32_t*)vec1.data;   // This cast has zero overhead. 

    vec1_data[0] = 2;                           // vec1_data can be used as a normal int32_t array
    vec1_data[1] = 4;

    vec1_data[2] = vec1_data[0] * vec1_data[1]; 
}