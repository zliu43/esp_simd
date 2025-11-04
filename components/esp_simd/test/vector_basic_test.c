#include "vector.h"
#include "vector_basic_functions.h"
#include "scalar_basic_functions.h"
#include "vector_test_helper.h"
#include "vector_basic_test.h" 
#include "esp_log.h"
#include <stdlib.h> 
#include <math.h>  

// TODO: write test function dispatcher 

void vector_test_add(bool verbose, dtype type){  
    timer_init();
    set_seed(42);

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 512;                                            // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors
        vector_t *vec2 = create_test_vector(test_size, type);
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec2->size, vec2->type); 

        assert(vec1);                                                   // Check if valid
        assert(vec2);
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range
        fill_test_vector(vec2); 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs)
        vector_t *vec2_copy = vector_create(vec2->size, vec2->type); 
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_add(vec1, vec2, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_add(vec1, vec2, simd_result);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs
        vector_assert_eq(vec2, vec2_copy);
        vector_check_canary(vec1);                                      // Check modification of canary region
        vector_check_canary(vec2);
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources
        vector_destroy(vec2);
        vector_destroy(vec1_copy);
        vector_destroy(vec2_copy);
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_add", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_add", "scalar_time: %d", scalar_time);
    }
}

void vector_test_add_alias(bool verbose, dtype type) {
    timer_init();
    set_rand_seed();
    uint32_t vec_time = 0;
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++) {
        int test_size = 1 + rand() % MAX_SIZE;
        vector_t *vec1 = create_test_vector(test_size, type);
        vector_t *vec2 = create_test_vector(test_size, type);

        fill_test_vector(vec1);
        fill_test_vector(vec2);
 
        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);
        vector_t *vec2_copy = vector_create(vec2->size, vec2->type);
        vec_copy(vec1, vec1_copy);
        vec_copy(vec2, vec2_copy);

        assert(vector_assert_eq(vec1, vec1_copy));
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();
        vec_add(vec1, vec2, vec1);    
        timer_end(&vec_time);
 
        timer_start();
        scalar_add(vec1_copy, vec2, vec2);  
        timer_end(&scalar_time);
 
        assert(vector_assert_eq(vec1, vec2)); 
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2)); 
 
        vector_destroy(vec1);
        vector_destroy(vec2);
        vector_destroy(vec1_copy);
        vector_destroy(vec2_copy); 
    }

    timer_deinit();

    if (verbose) {
        ESP_LOGI("vector_test_add_alias", "vector_time: %d", vec_time);
        ESP_LOGI("vector_test_add_alias", "scalar_time: %d", scalar_time);
    }
}

void vector_test_sub(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors
        vector_t *vec2 = create_test_vector(test_size, type);
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec2->size, vec2->type); 

        assert(vec1);                                                   // Check if valid
        assert(vec2);
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range
        fill_test_vector(vec2); 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs)
        vector_t *vec2_copy = vector_create(vec2->size, vec2->type); 
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_sub(vec1, vec2, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_sub(vec1, vec2, simd_result);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs
        vector_assert_eq(vec2, vec2_copy);
        vector_check_canary(vec1);                                      // Check modification of canary region
        vector_check_canary(vec2);
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources
        vector_destroy(vec2);
        vector_destroy(vec1_copy);
        vector_destroy(vec2_copy);
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_sub", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_sub", "scalar_time: %d", scalar_time);
    }
}

void vector_test_sub_alias(bool verbose, dtype type) {
    timer_init();
    set_rand_seed();
    uint32_t vec_time = 0;
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++) {
        int test_size = 1 + rand() % MAX_SIZE;
        vector_t *vec1 = create_test_vector(test_size, type);
        vector_t *vec2 = create_test_vector(test_size, type);

        fill_test_vector(vec1);
        fill_test_vector(vec2);
 
        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);
        vector_t *vec2_copy = vector_create(vec2->size, vec2->type);
        vec_copy(vec1, vec1_copy);
        vec_copy(vec2, vec2_copy);

        assert(vector_assert_eq(vec1, vec1_copy));
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();
        vec_sub(vec1, vec2, vec1);    
        timer_end(&vec_time);
 
        timer_start();
        scalar_sub(vec1_copy, vec2, vec2);  
        timer_end(&scalar_time);
 
        assert(vector_assert_eq(vec1, vec2)); 
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2)); 
 
        vector_destroy(vec1);
        vector_destroy(vec2);
        vector_destroy(vec1_copy);
        vector_destroy(vec2_copy); 
    }

    timer_deinit();

    if (verbose) {
        ESP_LOGI("vector_test_sub_alias", "vector_time: %d", vec_time);
        ESP_LOGI("vector_test_sub_alias", "scalar_time: %d", scalar_time);
    }
}

void vector_test_add_scalar(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;
    
    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int rand_scalar; 
        switch (type) {
            case DTYPE_INT8:   rand_scalar = (rand() % 256) - 128;      break;
            case DTYPE_INT16:  rand_scalar = (rand() % 65536) - 32768;  break;
            default:           rand_scalar = rand();                    break;
        }
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        assert(vec1_copy);
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_add_scalar(vec1, rand_scalar, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_add_scalar(vec1, rand_scalar, simd_result);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_add_scalar", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_add_scalar", "scalar_time: %d", scalar_time);
    }
}

void vector_test_add_scalar_f32(bool verbose, dtype type){ 
    assert(type == DTYPE_FLOAT32);
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;
    
    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        float rand_scalar = rand_float_val();  
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        assert(vec1_copy);
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_add_scalar_f32(vec1, rand_scalar, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_add_scalar_f32(vec1, rand_scalar, simd_result);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_add_scalar", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_add_scalar", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_add_scalar_alias(bool verbose, dtype type) {
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;     
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++) { 
        int rand_scalar;
        switch (type) {
            case DTYPE_INT8:   rand_scalar = (rand() % 256) - 128;      break;
            case DTYPE_INT16:  rand_scalar = (rand() % 65536) - 32768;  break;
            default:           rand_scalar = rand();                    break;
        }
 
        int test_size = 1 + rand() % MAX_SIZE;
 
        vector_t *vec_in = create_test_vector(test_size, type);
        assert(vec_in);
        fill_test_vector(vec_in);
 
        vector_t *expected = create_test_vector(vec_in->size, vec_in->type);
        assert(expected);
        vec_copy(vec_in, expected);
 
        assert(vector_assert_eq(vec_in, expected));
        assert(vector_check_canary(vec_in));
        assert(vector_check_canary(expected));
 
        timer_start();
        scalar_add_scalar(expected, rand_scalar, expected);
        timer_end(&scalar_time);
 
        timer_start();
        vec_add_scalar(vec_in, rand_scalar, vec_in);
        timer_end(&vec_time);
 
        vector_assert_eq(vec_in, expected);
 
        vector_check_canary(vec_in);
        vector_check_canary(expected);
 
        vector_destroy(vec_in);
        vector_destroy(expected);
    }

    timer_deinit();

    if (verbose) {
        ESP_LOGI("vector_test_add_scalar_aliasing", "vec_time: %u", vec_time);
        ESP_LOGI("vector_test_add_scalar_aliasing", "scalar_time: %u", scalar_time);
    }
}
 
void vector_test_add_scalar_f32_alias(bool verbose, dtype type) {
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;     
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++) { 
        float rand_scalar = rand_float_val();
 
        int test_size = 1 + rand() % MAX_SIZE;
 
        vector_t *vec_in = create_test_vector(test_size, type);
        assert(vec_in);
        fill_test_vector(vec_in);
 
        vector_t *expected = create_test_vector(vec_in->size, vec_in->type);
        assert(expected);
        vec_copy(vec_in, expected);
 
        assert(vector_assert_eq(vec_in, expected));
        assert(vector_check_canary(vec_in));
        assert(vector_check_canary(expected));
 
        timer_start();
        scalar_add_scalar_f32(expected, rand_scalar, expected);
        timer_end(&scalar_time);
 
        timer_start();
        vec_add_scalar_f32(vec_in, rand_scalar, vec_in);
        timer_end(&vec_time);
 
        vector_assert_eq(vec_in, expected);
 
        vector_check_canary(vec_in);
        vector_check_canary(expected);
 
        vector_destroy(vec_in);
        vector_destroy(expected);
    }

    timer_deinit();

    if (verbose) {
        ESP_LOGI("vector_test_add_scalar_aliasing", "vec_time: %u", vec_time);
        ESP_LOGI("vector_test_add_scalar_aliasing", "scalar_time: %u", scalar_time);
    }
}

void vector_test_mul_shift(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;
    unsigned int rand_shift_amount;

    switch (type) {
        case DTYPE_INT8:   rand_shift_amount = (rand() % 8);    break;
        case DTYPE_INT16:  rand_shift_amount = (rand() % 16);   break;
        default:           rand_shift_amount = (rand() % 32);   break;
    }

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                              // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors
        vector_t *vec2 = create_test_vector(test_size, type);
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec2->size, vec2->type); 

        assert(vec1);                                                   // Check if valid
        assert(vec2);
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range
        fill_test_vector(vec2); 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs)
        vector_t *vec2_copy = vector_create(vec2->size, vec2->type); 
        assert(vec1_copy);
        assert(vec2_copy);
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_mul(vec1, vec2, scalar_result, rand_shift_amount) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_mul(vec1, vec2, simd_result,  rand_shift_amount) == VECTOR_SUCCESS);
        timer_end(&vec_time);  

        assert(vector_assert_eq(simd_result, scalar_result));                   // Check results
        assert(vector_assert_eq(vec1, vec1_copy));                              // Check modification of inputs
        assert(vector_assert_eq(vec2, vec2_copy));
        vector_check_canary(vec1);                                      // Check modification of canary region
        vector_check_canary(vec2);
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources
        vector_destroy(vec2);
        vector_destroy(vec1_copy);
        vector_destroy(vec2_copy);
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_mul_shift", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_mul_shift", "scalar_time: %d", scalar_time);
    }
} 

void vector_test_mul_shift_alias(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0; 

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){

        unsigned int rand_shift_amount;

        switch (type) {
            case DTYPE_INT8:   rand_shift_amount = (rand() % 8);    break;
            case DTYPE_INT16:  rand_shift_amount = (rand() % 16);   break;
            default:           rand_shift_amount = (rand() % 32);   break;
        }

        int test_size = 1 + rand() % MAX_SIZE;                              // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors
        vector_t *vec2 = create_test_vector(test_size, type); 

        assert(vec1);                                                   // Check if valid
        assert(vec2); 

        fill_test_vector(vec1);                                         // Fill with random values in range
        fill_test_vector(vec2); 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs)
        vector_t *vec2_copy = vector_create(vec2->size, vec2->type); 
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_mul(vec1, vec2, vec1, rand_shift_amount);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_mul(vec1_copy, vec2, vec2, rand_shift_amount);
        timer_end(&vec_time); 
 
        vector_check_canary(vec1);                                      // Check modification of canary region
        vector_check_canary(vec2); 
  
        vector_destroy(vec1);                                           // Free resources
        vector_destroy(vec2);
        vector_destroy(vec1_copy);
        vector_destroy(vec2_copy); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_mul_shift", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_mul_shift", "scalar_time: %d", scalar_time);
    }
}

void vector_test_sum(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors  
        int32_t vector_sum_val;
        int32_t scalar_sum_val;

        assert(vec1);              
        fill_test_vector(vec1);                                         // Fill with random values in range 
        if (type == DTYPE_INT32){                                       // Behavior undefined for overflow.
            int32_t* data = vec1->data;
            for (int i = 0; i < vec1->size; i++){
                int32_t val = rand() % UINT16_MAX + INT16_MIN;
                data[i] = val;
            }
        }

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_sum(vec1, &scalar_sum_val) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_sum(vec1, &vector_sum_val) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
  
        if(vector_sum_val != scalar_sum_val){
            ESP_LOGE("vector_test_sum", "Sum mismatch: vector_sum: %d, scalar_sum: %d", (int)vector_sum_val, (int)scalar_sum_val);
            assert(0);
        }
        vector_check_canary(vec1);                                      // Check modification of canary region 
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_sum", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_sum", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_sum_f32(bool verbose, dtype type){ 
    assert(type == DTYPE_FLOAT32);
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                              // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors  
        float vector_sum;
        float scalar_sum;

        assert(vec1);                

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_sum_f32(vec1, &scalar_sum) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_sum_f32(vec1, &vector_sum) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
  
        if(!float_eq(vector_sum, scalar_sum)){
            ESP_LOGE("vector_test_sum", "Sum mismatch: vector_sum: %f, scalar_sum: %f", vector_sum, scalar_sum);
            assert(0);
        }
        vector_check_canary(vec1);                                      // Check modification of canary region 
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_sum", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_sum", "scalar_time: %d", scalar_time);
    }
}

void vector_test_mul_scalar_shift(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;
    unsigned int rand_shift_amount;

    switch (type) {
        case DTYPE_INT8:   rand_shift_amount = (rand() % 8);    break;
        case DTYPE_INT16:  rand_shift_amount = (rand() % 16);   break;
        default:           rand_shift_amount = (rand() % 32);   break;
    }

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                              // Random vector sizes 
        int rand_scalar;
        if (type != DTYPE_FLOAT32) { rand_scalar = rand_scalar_val(type);}
        float rand_float = rand_float_val();
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 
        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        assert(vec1_copy); 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        if (type == DTYPE_FLOAT32){
            assert(scalar_mul_scalar_f32(vec1, rand_float, scalar_result) == VECTOR_SUCCESS);
        } else{
            assert(scalar_mul_scalar(vec1, rand_scalar, scalar_result, rand_shift_amount) == VECTOR_SUCCESS); 
        }
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        if (type == DTYPE_FLOAT32){
            assert(vec_mul_scalar_f32(vec1, rand_float, simd_result) == VECTOR_SUCCESS);
        } else{
            assert(vec_mul_scalar(vec1, rand_scalar, simd_result, rand_shift_amount) == VECTOR_SUCCESS);
        }
        timer_end(&vec_time);  

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_mul_scalar", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_mul_scalar", "scalar_time: %d", scalar_time);
    }
} 

void vector_test_dotp(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                              // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors
        vector_t *vec2 = create_test_vector(test_size, type); 
        int32_t scalar_result = 0xDEADBEEF;
        int32_t simd_result = 0xDEADBEEF;

        assert(vec1);                                                   // Check if valid
        assert(vec2); 

        fill_test_vector(vec1);                                         // Fill with random values in range
        fill_test_vector(vec2); 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs)
        vector_t *vec2_copy = vector_create(vec2->size, vec2->type); 
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_dotp(vec1, vec2, &scalar_result) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_dotp(vec1, vec2, &simd_result) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
 
        assert( scalar_result == simd_result);
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs
        vector_assert_eq(vec2, vec2_copy);
        vector_check_canary(vec1);                                      // Check modification of canary region
        vector_check_canary(vec2); 
  
        vector_destroy(vec1);                                           // Free resources
        vector_destroy(vec2);
        vector_destroy(vec1_copy);
        vector_destroy(vec2_copy); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_dotp", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_dotp", "scalar_time: %d", scalar_time);
    }
}

void vector_test_dotp_f32(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                              // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors
        vector_t *vec2 = create_test_vector(test_size, type); 
        float scalar_result;
        float simd_result;

        assert(vec1);                                                   // Check if valid
        assert(vec2); 

        fill_test_vector(vec1);                                         // Fill with random values in range
        fill_test_vector(vec2); 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs)
        vector_t *vec2_copy = vector_create(vec2->size, vec2->type); 
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_dotp_f32(vec1, vec2, &scalar_result) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_dotp_f32(vec1, vec2, &simd_result) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
  
        assert(float_eq(scalar_result, simd_result));
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs
        vector_assert_eq(vec2, vec2_copy);
        vector_check_canary(vec1);                                      // Check modification of canary region
        vector_check_canary(vec2); 
  
        vector_destroy(vec1);                                           // Free resources
        vector_destroy(vec2);
        vector_destroy(vec1_copy);
        vector_destroy(vec2_copy); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_dotp", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_dotp", "scalar_time: %d", scalar_time);
    }
}

void vector_test_abs(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                              // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_abs(vec1, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_abs(vec1, simd_result);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_abs", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_abs", "scalar_time: %d", scalar_time);
    }
}

void vector_test_ceil(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 
        int rand_scalar = rand_scalar_val(type);

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_ceil(vec1, scalar_result, rand_scalar);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_ceil(vec1, simd_result, rand_scalar);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_ceil", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_ceil", "scalar_time: %d", scalar_time);
    }
}

void vector_test_ceil_f32(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();
    assert(type == DTYPE_FLOAT32);

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 
        float rand_float = rand_float_val(type);

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_ceil_f32(vec1, scalar_result, rand_float);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_ceil_f32(vec1, simd_result, rand_float);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_ceil_f32", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_ceil_f32", "scalar_time: %d", scalar_time);
    }
}

void vector_test_floor(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 
        int rand_scalar = rand_scalar_val(type);

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_floor(vec1, scalar_result, rand_scalar);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_floor(vec1, simd_result, rand_scalar);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_floor", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_floor", "scalar_time: %d", scalar_time);
    }
}

void vector_test_floor_f32(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();
    assert(type == DTYPE_FLOAT32);

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 
        float rand_float = rand_float_val(type);

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_floor_f32(vec1, scalar_result, rand_float);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_floor_f32(vec1, simd_result, rand_float);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_floor_f32", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_floor_f32", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_neg(bool verbose, dtype type){  
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                              // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_neg(vec1, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_neg(vec1, simd_result);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_neg", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_neg", "scalar_time: %d", scalar_time);
    }
}

void vector_test_mac(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors  
        int32_t rand_start_val = rand_scalar_val(type);
        int rand_multiplier = rand_scalar_val(type);
        int32_t vector_accumulator = rand_start_val;
        int32_t scalar_accumulator = rand_start_val;

        assert(vec1);              
        fill_test_vector(vec1);                                         // Fill with random values in range 
        if (type == DTYPE_INT32){                                       // Behavior undefined for overflow.
            int32_t* data = vec1->data;
            for (int i = 0; i < vec1->size; i++){
                int32_t val = rand() % UINT16_MAX + INT16_MIN;
                data[i] = val;
            } 
        }

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_mac(vec1, &scalar_accumulator, rand_multiplier) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_mac(vec1, &vector_accumulator,rand_multiplier) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
  
        if(vector_accumulator != scalar_accumulator){
            ESP_LOGE("vector_test_mac", "Accumulator mismatch: vector_accumulator: %d, scalar_accumulator: %d", (int)vector_accumulator, (int)scalar_accumulator);
            assert(0);
        }
        vector_check_canary(vec1);                                      // Check modification of canary region 
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_mac", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_mac", "scalar_time: %d", scalar_time);
    }
}

void vector_test_mac_f32(bool verbose, dtype type){ 

    timer_init();
    set_seed(42);

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < 128; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors  
        float rand_start_val = rand_float_val();
        float rand_multiplier = rand_float_val();
        float vector_accumulator = rand_start_val;
        float scalar_accumulator = rand_start_val;

        assert(vec1);              
        fill_test_vector(vec1);                                         // Fill with random values in range  

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_mac_f32(vec1, &scalar_accumulator, rand_multiplier) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_mac_f32(vec1, &vector_accumulator,rand_multiplier) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
  
        if(!float_eq(vector_accumulator, scalar_accumulator)){
            ESP_LOGE("vector_test_mac_f32", "Accumulator mismatch: vector_accumulator: %f, scalar_accumulator: %f", vector_accumulator, scalar_accumulator);
            assert(0);
        }
        vector_check_canary(vec1);                                      // Check modification of canary region 
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_mac_f32", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_mac_f32", "scalar_time: %d", scalar_time);
    }
}

void vector_test_zeros(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes  
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors  
        vector_t *vec2 = create_test_vector(test_size, type);           // Allocating the test vectors  

        assert(vec1);   
        assert(vec2);               
        assert(vector_check_canary(vec1)); 
        assert(vector_check_canary(vec2)); 
 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_zeros(vec1) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_zeros(vec2) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
  
        vector_assert_eq(vec1,vec2);
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(vec2);                                      // Check modification of canary region 
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec2); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_zeros", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_zeros", "scalar_time: %d", scalar_time);
    }
}

void vector_test_ones(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes  
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors  
        vector_t *vec2 = create_test_vector(test_size, type);           // Allocating the test vectors  

        assert(vec1);   
        assert(vec2);               
        assert(vector_check_canary(vec1)); 
        assert(vector_check_canary(vec2)); 
 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_ones(vec1) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_ones(vec2) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
  
        vector_assert_eq(vec1,vec2);
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(vec2);                                      // Check modification of canary region 
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec2); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_ones", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_ones", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_fill(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes  
        int rand_scalar = rand_scalar_val(type);
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors  
        vector_t *vec2 = create_test_vector(test_size, type);           // Allocating the test vectors  

        assert(vec1);   
        assert(vec2);               
        assert(vector_check_canary(vec1)); 
        assert(vector_check_canary(vec2)); 
 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_fill(vec1,rand_scalar) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_fill(vec2,rand_scalar) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
  
        vector_assert_eq(vec1,vec2);
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(vec2);                                      // Check modification of canary region 
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec2); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_fill", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_fill", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_fill_f32(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes  
        const float rand_scalar = rand_float_val();
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors  
        vector_t *vec2 = create_test_vector(test_size, type);           // Allocating the test vectors  

        assert(vec1);   
        assert(vec2);               
        assert(vector_check_canary(vec1)); 
        assert(vector_check_canary(vec2)); 
 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_fill_f32(vec1, rand_scalar) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_fill_f32(vec2, rand_scalar) == VECTOR_SUCCESS);
        timer_end(&vec_time); 
   
        vector_assert_eq(vec1,vec2);
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(vec2);                                      // Check modification of canary region 
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec2); 
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_fill_f32", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_fill_f32", "scalar_time: %d", scalar_time);
    }
}

void vector_test_copy(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors 
        vector_t *simd_result = create_test_vector(vec1->size, vec1->type); 
        vector_t *scalar_result = create_test_vector(vec1->size, vec1->type); 

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_copy(vec1, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_copy(vec1, simd_result);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_copy", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_copy", "scalar_time: %d", scalar_time);
    }
}

void vector_test_convert(bool verbose, dtype type, dtype target_type){
    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

    for (int run_num = 0; run_num < TEST_RUNS; run_num++){
        int test_size = 1 + rand() % MAX_SIZE;                          // Random vector sizes 
        vector_t *vec1 = create_test_vector(test_size, type);           // Allocating the test vectors   
        vector_t *simd_result = create_test_vector(vec1->size, target_type); 
        vector_t *scalar_result = create_test_vector(vec1->size, target_type); 

        assert(vec1);                                                   // Check if valid 
        assert(simd_result);
        assert(scalar_result);

        fill_test_vector(vec1);                                         // Fill with random values in range 

        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification of inputs) 
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        assert(scalar_convert(vec1, scalar_result) == VECTOR_SUCCESS);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        assert(vec_convert(vec1, simd_result) == VECTOR_SUCCESS);
        timer_end(&vec_time); 

        vector_assert_eq(simd_result, scalar_result);                   // Check results
        vector_assert_eq(vec1, vec1_copy);                              // Check modification of inputs 
        vector_check_canary(vec1);                                      // Check modification of canary region 
        vector_check_canary(simd_result);
        vector_check_canary(scalar_result);
  
        vector_destroy(vec1);                                           // Free resources 
        vector_destroy(vec1_copy); 
        vector_destroy(simd_result);
        vector_destroy(scalar_result);
    } 
    timer_deinit();
    if (verbose){
            ESP_LOGI("vector_test_convert", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_convert", "scalar_time: %d", scalar_time);
    }
}
