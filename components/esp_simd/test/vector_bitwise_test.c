#include "vector.h"
#include "vector_bitwise_functions.h"
#include "scalar_bitwise_functions.h"
#include "vector_test_helper.h"
#include "vector_basic_functions.h"
#include "vector_bitwise_test.h" 
#include "esp_log.h"
#include <stdlib.h> 
#include <math.h>

void vector_test_and(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

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
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_and(vec1, vec2, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_and(vec1, vec2, simd_result);
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
            ESP_LOGI("vector_test_and", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_and", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_and_alias(bool verbose, dtype type) {
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
        vec_and(vec1, vec2, vec1);    
        timer_end(&vec_time);
 
        timer_start();
        scalar_and(vec1_copy, vec2, vec2);  
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
        ESP_LOGI("vector_test_and_alias", "vector_time: %d", vec_time);
        ESP_LOGI("vector_test_and_alias", "scalar_time: %d", scalar_time);
    }
}

void vector_test_xor(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

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
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_xor(vec1, vec2, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_xor(vec1, vec2, simd_result);
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
            ESP_LOGI("vector_test_xor", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_xor", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_xor_alias(bool verbose, dtype type) {
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
        vec_xor(vec1, vec2, vec1);    
        timer_end(&vec_time);
 
        timer_start();
        scalar_xor(vec1_copy, vec2, vec2);  
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
        ESP_LOGI("vector_test_xor_alias", "vector_time: %d", vec_time);
        ESP_LOGI("vector_test_xor_alias", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_or(bool verbose, dtype type){ 

    timer_init();
    set_rand_seed();

    uint32_t vec_time = 0;                                              // Runtime logs
    uint32_t scalar_time = 0;

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
        vec_copy(vec1, vec1_copy); 
        vec_copy(vec2, vec2_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions
        assert(vector_assert_eq(vec2, vec2_copy));
        assert(vector_check_canary(vec1));
        assert(vector_check_canary(vec2));
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_or(vec1, vec2, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_or(vec1, vec2, simd_result);
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
            ESP_LOGI("vector_test_or", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_or", "scalar_time: %d", scalar_time);
    }
}
 
void vector_test_or_alias(bool verbose, dtype type) {
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
        vec_or(vec1, vec2, vec1);    
        timer_end(&vec_time);
 
        timer_start();
        scalar_or(vec1_copy, vec2, vec2);  
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
        ESP_LOGI("vector_test_and_alias", "vector_time: %d", vec_time);
        ESP_LOGI("vector_test_and_alias", "scalar_time: %d", scalar_time);
    }
}

void vector_test_not(bool verbose, dtype type){ 

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
        vector_t *vec1_copy = vector_create(vec1->size, vec1->type);    // Creating copies (to check for modification
        vec_copy(vec1, vec1_copy);  

        assert(vector_assert_eq(vec1, vec1_copy));                      // Checking copies, canary regions 
        assert(vector_check_canary(vec1)); 
 
        timer_start();                                                  // Scalar functions are assumed intended behavior
        scalar_not(vec1, scalar_result);
        timer_end(&scalar_time);

        timer_start();                                                  // Running tests
        vec_not(vec1, simd_result);
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
            ESP_LOGI("vector_test_not", "vector_time: %d", vec_time);
            ESP_LOGI("vector_test_not", "scalar_time: %d", scalar_time);
    }
}