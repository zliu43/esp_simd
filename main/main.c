#include "vector.h" 
#include "vector_basic_test.h"
#include "vector_bitwise_test.h"
#include "esp_task_wdt.h"   
#include "esp_log.h" 

void test_basic(void){
    // vec_add 
    ESP_LOGI("vector_test_add:", "DTYPE_INT8");
    vector_test_add(true, DTYPE_INT8);
    ESP_LOGI("vector_test_add:", "DTYPE_INT16");
    vector_test_add(true, DTYPE_INT16);
    ESP_LOGI("vector_test_add:", "DTYPE_INT32");
    vector_test_add(true, DTYPE_INT32);
    ESP_LOGI("vector_test_add_alias:", "DTYPE_INT32");
    vector_test_add_alias(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_add:", "DTYPE_FLOAT32");
    vector_test_add(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_add_alias:", "DTYPE_FLOAT32");
    vector_test_add_alias(true, DTYPE_FLOAT32);

    // vec_sub 
    ESP_LOGI("vector_test_sub:", "DTYPE_INT8");
    vector_test_sub(true, DTYPE_INT8);
    ESP_LOGI("vector_test_sub_alias:", "DTYPE_INT8");
    vector_test_sub_alias(true, DTYPE_INT8);
    ESP_LOGI("vector_test_sub:", "DTYPE_INT16");
    vector_test_sub(true, DTYPE_INT16);
    ESP_LOGI("vector_test_sub_alias:", "DTYPE_INT16");
    vector_test_sub_alias(true, DTYPE_INT16);
    ESP_LOGI("vector_test_sub:", "DTYPE_INT32");
    vector_test_sub(true, DTYPE_INT32);
    ESP_LOGI("vector_test_sub_alias:", "DTYPE_INT32");
    vector_test_sub_alias(true, DTYPE_INT32);
    ESP_LOGI("vector_test_sub:", "DTYPE_FLOAT32");
    vector_test_sub(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_sub_alias:", "DTYPE_FLOAT32");
    vector_test_sub_alias(true, DTYPE_FLOAT32);

    // vec_add_scalar 
    ESP_LOGI("vec_add_scalar:", "DTYPE_INT8");
    vector_test_add_scalar(true, DTYPE_INT8);
    ESP_LOGI("vec_add_scalar_alias:", "DTYPE_INT8");
    vector_test_add_scalar_alias(true, DTYPE_INT8);
    ESP_LOGI("vec_add_scalar:", "DTYPE_INT16");
    vector_test_add_scalar(true, DTYPE_INT16);
    ESP_LOGI("vec_add_scalar_alias:", "DTYPE_INT16");
    vector_test_add_scalar_alias(true, DTYPE_INT16);
    ESP_LOGI("vec_add_scalar:", "DTYPE_INT32");
    vector_test_add_scalar(true, DTYPE_INT32);
    ESP_LOGI("vec_add_scalar_alias:", "DTYPE_INT32");
    vector_test_add_scalar_alias(true, DTYPE_INT32); 
    ESP_LOGI("vec_add_scalar:", "DTYPE_FLOAT32");
    vector_test_add_scalar_f32(true, DTYPE_FLOAT32);
    ESP_LOGI("vec_add_scalar_alias:", "DTYPE_FLOAT32");
    vector_test_add_scalar_f32_alias(true, DTYPE_FLOAT32);

    // vec_mul_shift
    ESP_LOGI("vec_mul_shift:", "DTYPE_INT8");
    vector_test_mul_shift(true, DTYPE_INT8);
    ESP_LOGI("vec_mul_shift:", "DTYPE_INT16");
    vector_test_mul_shift(true, DTYPE_INT16);
    ESP_LOGI("vec_mul_shift:", "DTYPE_INT32");
    vector_test_mul_shift(true, DTYPE_INT32);
    ESP_LOGI("vec_mul_shift:", "DTYPE_FLOAT32");
    vector_test_mul_shift(true, DTYPE_FLOAT32);
    ESP_LOGI("vec_mul_shift_alias:", "DTYPE_INT8");
    vector_test_mul_shift_alias(true, DTYPE_INT8);
    ESP_LOGI("vec_mul_shift_alias:", "DTYPE_INT16");
    vector_test_mul_shift_alias(true, DTYPE_INT16);
    ESP_LOGI("vec_mul_shift_alias:", "DTYPE_INT32");
    vector_test_mul_shift_alias(true, DTYPE_INT32);
    ESP_LOGI("vec_mul_shift_alias:", "DTYPE_FLOAT32");
    vector_test_mul_shift_alias(true, DTYPE_FLOAT32);
    
    // vec_sum
    
    ESP_LOGI("vec_sum:", "DTYPE_INT8");
    vector_test_sum(true, DTYPE_INT8);
    ESP_LOGI("vec_sum:", "DTYPE_INT16");
    vector_test_sum(true, DTYPE_INT16);
    ESP_LOGI("vec_sum:", "DTYPE_INT32");  
    vector_test_sum(true, DTYPE_INT32);
    ESP_LOGI("vec_sum:", "DTYPE_FLOAT32");
    vector_test_sum_f32(true, DTYPE_FLOAT32);
    
    
    // vec_mul_scalar_shift
    ESP_LOGI("vec_mul_scalar_shift:", "DTYPE_INT8");
    vector_test_mul_scalar_shift(true, DTYPE_INT8);
    ESP_LOGI("vec_mul_scalar_shift:", "DTYPE_INT16");
    vector_test_mul_scalar_shift(true, DTYPE_INT16);
    ESP_LOGI("vec_mul_scalar_shift:", "DTYPE_INT32");
    vector_test_mul_scalar_shift(true, DTYPE_INT32);
    ESP_LOGI("vec_mul_scalar_shift_f32:", "DTYPE_FLOAT32");
    vector_test_mul_scalar_shift(true, DTYPE_FLOAT32);

    // vec_dotp 
    ESP_LOGI("vec_dotp:", "DTYPE_INT8"); 
    vector_test_dotp(true, DTYPE_INT8);
    ESP_LOGI("vec_dotp:", "DTYPE_INT16"); 
    vector_test_dotp(true, DTYPE_INT16);
    ESP_LOGI("vec_dotp:", "DTYPE_INT32"); 
    vector_test_dotp(true, DTYPE_INT32);
    ESP_LOGI("vec_dotp_f32:", "DTYPE_FLOAT32"); 
    vector_test_dotp_f32(true, DTYPE_FLOAT32);

    // vec_abs
    ESP_LOGI("vec_abs:", "DTYPE_INT8"); 
    vector_test_abs(true, DTYPE_INT8);
    ESP_LOGI("vec_abs:", "DTYPE_INT16"); 
    vector_test_abs(true, DTYPE_INT16);
    ESP_LOGI("vec_abs:", "DTYPE_INT32"); 
    vector_test_abs(true, DTYPE_INT32);
    ESP_LOGI("vec_abs:", "DTYPE_FLOAT32"); 
    vector_test_abs(true, DTYPE_FLOAT32);

    // vec_ceil
    ESP_LOGI("vec_ceil:", "DTYPE_INT8"); 
    vector_test_ceil(true, DTYPE_INT8);
    ESP_LOGI("vec_ceil:", "DTYPE_INT16"); 
    vector_test_ceil(true, DTYPE_INT16);
    ESP_LOGI("vec_ceil:", "DTYPE_INT32"); 
    vector_test_ceil(true, DTYPE_INT32); 
    ESP_LOGI("vec_ceil_f32:", "DTYPE_FLOAT32"); 
    vector_test_ceil_f32(true, DTYPE_FLOAT32); 
    
     // vec_floor
    ESP_LOGI("vec_floor:", "DTYPE_INT8"); 
    vector_test_floor(true, DTYPE_INT8);
    ESP_LOGI("vec_floor:", "DTYPE_INT16"); 
    vector_test_floor(true, DTYPE_INT16);
    ESP_LOGI("vec_floor:", "DTYPE_INT32"); 
    vector_test_floor(true, DTYPE_INT32); 
    ESP_LOGI("vec_floor:", "DTYPE_FLOAT32"); 
    vector_test_floor_f32(true, DTYPE_FLOAT32); 

    // vec_neg
    ESP_LOGI("vec_neg:", "DTYPE_INT8"); 
    vector_test_neg(true, DTYPE_INT8);
    ESP_LOGI("vec_neg:", "DTYPE_INT16"); 
    vector_test_neg(true, DTYPE_INT16);
    ESP_LOGI("vec_neg:", "DTYPE_INT32"); 
    vector_test_neg(true, DTYPE_INT32);
    ESP_LOGI("vec_neg:", "DTYPE_FLOAT32"); 
    vector_test_neg(true, DTYPE_FLOAT32);


    // vec_mac
    ESP_LOGI("vec_mac:", "DTYPE_INT8"); 
    vector_test_mac(true, DTYPE_INT8);
    ESP_LOGI("vec_mac:", "DTYPE_INT16"); 
    vector_test_mac(true, DTYPE_INT16);
    ESP_LOGI("vec_mac:", "DTYPE_INT32"); 
    vector_test_mac(true, DTYPE_INT32);
    ESP_LOGI("vec_mac_f32:", "DTYPE_FLOAT32"); 
    vector_test_mac_f32(true, DTYPE_FLOAT32);


    // vec_zeros
    ESP_LOGI("vec_zero:", "DTYPE_INT8"); 
    vector_test_zeros(true, DTYPE_INT8);
    ESP_LOGI("vec_zero:", "DTYPE_INT16"); 
    vector_test_zeros(true, DTYPE_INT16);
    ESP_LOGI("vec_zero:", "DTYPE_INT32"); 
    vector_test_zeros(true, DTYPE_INT32);
    ESP_LOGI("vec_zero:", "DTYPE_FLOAT32"); 
    vector_test_zeros(true, DTYPE_FLOAT32);

    // vec_ones
    ESP_LOGI("vec_ones:", "DTYPE_INT8"); 
    vector_test_ones(true, DTYPE_INT8);
    ESP_LOGI("vec_ones:", "DTYPE_INT16"); 
    vector_test_ones(true, DTYPE_INT16);
    ESP_LOGI("vec_ones:", "DTYPE_INT32"); 
    vector_test_ones(true, DTYPE_INT32);
    ESP_LOGI("vec_ones:", "DTYPE_FLOAT32"); 
    vector_test_ones(true, DTYPE_FLOAT32);

    // vec_fill
    ESP_LOGI("vec_fill:", "DTYPE_INT8"); 
    vector_test_fill(true, DTYPE_INT8);
    ESP_LOGI("vec_fill:", "DTYPE_INT16"); 
    vector_test_fill(true, DTYPE_INT16);
    ESP_LOGI("vec_fill:", "DTYPE_INT32"); 
    vector_test_fill(true, DTYPE_INT32);
    ESP_LOGI("vec_fill:", "DTYPE_FLOAT32"); 
    vector_test_fill_f32(true, DTYPE_FLOAT32);

    // vec_copy
    ESP_LOGI("vec_copy:", "DTYPE_INT8"); 
    vector_test_copy(true, DTYPE_INT8);
    ESP_LOGI("vec_copy:", "DTYPE_INT16"); 
    vector_test_copy(true, DTYPE_INT16);
    ESP_LOGI("vec_copy:", "DTYPE_INT32"); 
    vector_test_copy(true, DTYPE_INT32);
    ESP_LOGI("vec_copy:", "DTYPE_FLOAT32"); 
    vector_test_copy(true, DTYPE_FLOAT32);
}

void test_bitwise(void){
    // vec_and 
    ESP_LOGI("vector_test_and:", "DTYPE_INT8");
    vector_test_and(true, DTYPE_INT8);
    ESP_LOGI("vector_test_and:", "DTYPE_INT16");
    vector_test_and(true, DTYPE_INT16);
    ESP_LOGI("vector_test_and:", "DTYPE_INT32");
    vector_test_and(true, DTYPE_INT32);
    ESP_LOGI("vector_test_and_alias:", "DTYPE_INT32");
    vector_test_and_alias(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_and:", "DTYPE_FLOAT32");
    vector_test_and(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_and_alias:", "DTYPE_FLOAT32");
    vector_test_and_alias(true, DTYPE_FLOAT32);

    // vec_or 
    ESP_LOGI("vector_test_or:", "DTYPE_INT8");
    vector_test_or(true, DTYPE_INT8);
    ESP_LOGI("vector_test_or:", "DTYPE_INT16");
    vector_test_or(true, DTYPE_INT16);
    ESP_LOGI("vector_test_or:", "DTYPE_INT32");
    vector_test_or(true, DTYPE_INT32);
    ESP_LOGI("vector_test_or_alias:", "DTYPE_INT32");
    vector_test_or_alias(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_or:", "DTYPE_FLOAT32");
    vector_test_or(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_or_alias:", "DTYPE_FLOAT32");
    vector_test_or_alias(true, DTYPE_FLOAT32);

    // vec_xor 
    ESP_LOGI("vector_test_xor:", "DTYPE_INT8");
    vector_test_xor(true, DTYPE_INT8);
    ESP_LOGI("vector_test_xor:", "DTYPE_INT16");
    vector_test_xor(true, DTYPE_INT16);
    ESP_LOGI("vector_test_xor:", "DTYPE_INT32");
    vector_test_xor(true, DTYPE_INT32);
    ESP_LOGI("vector_test_xor_alias:", "DTYPE_INT32");
    vector_test_xor_alias(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_xor:", "DTYPE_FLOAT32");
    vector_test_xor(true, DTYPE_FLOAT32);
    ESP_LOGI("vector_test_xor_alias:", "DTYPE_FLOAT32");
    vector_test_xor_alias(true, DTYPE_FLOAT32);

    // vec_not 
    ESP_LOGI("vector_test_not:", "DTYPE_INT8");
    vector_test_not(true, DTYPE_INT8);
    ESP_LOGI("vector_test_not:", "DTYPE_INT16");
    vector_test_not(true, DTYPE_INT16);
    ESP_LOGI("vector_test_not:", "DTYPE_INT32");
    vector_test_not(true, DTYPE_INT32); 
    ESP_LOGI("vector_test_not:", "DTYPE_FLOAT32");
    vector_test_not(true, DTYPE_FLOAT32); 
}

void app_main(void)
{ 
    VECTOR_STACK_INIT(vec1, 8, DTYPE_INT32);
    int32_t* vec1_data = (int32_t*)vec1.data;
    vec1_data[1] = 4;
}
