#include "scalar_functions.h"
#include "stdlib.h"
#include "vector.h"

void scalar_add_i8(const vector_t *vec1,const  vector_t *vec2, vector_t *result) {
    assert(vec1->type == vec2->type && vec1->type == result->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == vec2->size && vec1->size == result->size);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *vec2_data = (int8_t *)vec2->data;
    int8_t *result_data = (int8_t *)result->data;

    const int SAT_MAX = 127;
    const int SAT_MIN = -128;

    for (int i = 0; i < vec1->size; i++) {
        int sum = vec1_data[i] + vec2_data[i];
        if (sum > SAT_MAX) sum = SAT_MAX;
        else if (sum < SAT_MIN) sum = SAT_MIN;
        result_data[i] = (int8_t)sum;
    }
}

void scalar_add_i8_scalar(const vector_t *vec1, const int scalar, vector_t *result) {
    assert(vec1->size == result->size && vec1->type == result->type && vec1->type == DTYPE_INT8); 

    int8_t *vec1_data = (int8_t *)vec1->data; 
    int8_t *result_data = (int8_t *)result->data;

    const int SAT_MAX = 127;
    const int SAT_MIN = -128;

    for (int i = 0; i < vec1->size; i++) {
        int sum = vec1_data[i] + scalar;
        if (sum > SAT_MAX) sum = SAT_MAX;
        else if (sum < SAT_MIN) sum = SAT_MIN;
        result_data[i] = (int8_t)sum;
    }
}

void scalar_sub_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result) {
    assert(vec1->type == vec2->type && vec1->type == result->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == vec2->size && vec1->size == result->size);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *vec2_data = (int8_t *)vec2->data;
    int8_t *result_data = (int8_t *)result->data;

    const int SAT_MAX = 127;
    const int SAT_MIN = -128;

    for (int i = 0; i < vec1->size; i++) {
        int sum = vec1_data[i] - vec2_data[i];
        if (sum > SAT_MAX) sum = SAT_MAX;
        else if (sum < SAT_MIN) sum = SAT_MIN;
        result_data[i] = (int8_t)sum;
    }
}

void scalar_abs_i8(const vector_t *vec1, vector_t *result){
    assert(vec1->size == result->size && vec1->type == result->type && vec1->type == DTYPE_INT8); 

    int8_t *vec1_data = (int8_t *)vec1->data; 
    int8_t *result_data = (int8_t *)result->data;

    const int SAT_MAX = 127;
    const int SAT_MIN = -128;

    for (int i = 0; i < vec1->size; i++) {
        int intermediate = vec1_data[i] == SAT_MIN ? SAT_MAX : vec1_data[i]; 
        intermediate = intermediate < 0 ? -intermediate : intermediate;
        result_data[i] = (int8_t)intermediate;
    }
}

void scalar_ceil_i8(const vector_t *vec1, vector_t *result, const int ceiling){
    assert(vec1->size == result->size && vec1->type == result->type && vec1->type == DTYPE_INT8); 

    int8_t *vec1_data = (int8_t *)vec1->data; 
    int8_t *result_data = (int8_t *)result->data;

    for (int i = 0; i < vec1->size; i++) {
        int intermediate = vec1_data[i] > ceiling ? ceiling : vec1_data[i];  
        result_data[i] = (int8_t)intermediate;
    }
}

void scalar_floor_i8(const vector_t *vec1, vector_t *result, const int floor){
    assert(vec1->size == result->size && vec1->type == result->type && vec1->type == DTYPE_INT8); 

    int8_t *vec1_data = (int8_t *)vec1->data; 
    int8_t *result_data = (int8_t *)result->data;

    for (int i = 0; i < vec1->size; i++) {
        int intermediate = vec1_data[i] < floor ? floor : vec1_data[i];  
        result_data[i] = (int8_t)intermediate;
    }
}

void scalar_compare_eq_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == vec2->size && vec1->size == result->size);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *vec2_data = (int8_t *)vec2->data;
    int8_t *result_data = (int8_t *)result->data; 

    for (int i = 0; i < vec1->size; i++) { 
        result_data[i] = vec1_data[i] == vec2_data[i] ? -1 : 0;
    }
}

void scalar_compare_gt_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == vec2->size && vec1->size == result->size);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *vec2_data = (int8_t *)vec2->data;
    int8_t *result_data = (int8_t *)result->data; 

    for (int i = 0; i < vec1->size; i++) { 
        result_data[i] = vec1_data[i] > vec2_data[i] ? -1 : 0;
    }
}

void scalar_compare_lt_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->type == vec2->type && vec1->type == result->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == vec2->size && vec1->size == result->size);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *vec2_data = (int8_t *)vec2->data;
    int8_t *result_data = (int8_t *)result->data; 

    for (int i = 0; i < vec1->size; i++) { 
        result_data[i] = vec1_data[i] < vec2_data[i] ? -1 : 0;
    }
}

void scalar_dotp_i8(const vector_t *vec1, const vector_t *vec2, int32_t *result){
    assert(vec1->type == vec2->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == vec2->size);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *vec2_data = (int8_t *)vec2->data;

    int32_t sum = 0;
    for (int i = 0; i < vec1->size; i++) {
        sum += vec1_data[i] * vec2_data[i];
    }
    *result = sum;
}

void scalar_neg_i8(const vector_t *vec1, vector_t *result) {
    assert(vec1->size == result->size && vec1->type == result->type && vec1->type == DTYPE_INT8);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *result_data = (int8_t *)result->data;

    for (int i = 0; i < vec1->size; i++) {
        if (vec1_data[i] == -128) {
            result_data[i] = 127; // Handle overflow case
        } else {
            result_data[i] = -vec1_data[i];
        } 
    }
}   

void scalar_mul_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result){
    assert(vec1->size == result->size && vec2->type == DTYPE_INT16);
    assert(vec1->size == vec2->size && vec1->type == vec2->type && vec2->type == DTYPE_INT8);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *vec2_data = (int8_t *)vec2->data;
    int16_t *result_data = (int16_t *)result->data;

    for (int i = 0; i < vec1->size; i++) {
        result_data[i] = (int16_t)(vec1_data[i] * vec2_data[i]);
    }
}

void scalar_mul_shift_i8(const vector_t *vec1, const vector_t *vec2, vector_t *result, const int shift_amount){
    assert(vec1->size == result->size && vec2->type == DTYPE_INT8);
    assert(vec1->size == vec2->size && vec1->type == vec2->type && vec2->type == DTYPE_INT8);
    assert(shift_amount >= 0 && shift_amount <= 8); // Ensure shift amount is within valid range for int8_t

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t *vec2_data = (int8_t *)vec2->data;
    int8_t *result_data = (int8_t *)result->data;

    for (int i = 0; i < vec1->size; i++) {
        int16_t intermediate = (int16_t)(vec1_data[i] * vec2_data[i]);
        result_data[i] = (int8_t)(intermediate >> shift_amount);
    }
}

void scalar_mac_i8(const vector_t *vec1, int32_t* accumulator, const int multiplier) {
    assert(vec1->type == DTYPE_INT8);
    assert(vec1->size > 0);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int8_t multiplier_i8 = (int8_t)multiplier;

    for (int i = 0; i < vec1->size; i++) {
        *accumulator += vec1_data[i] * multiplier_i8;
    }
}

void scalar_zero_i8(vector_t *vec) {
    assert(vec->type == DTYPE_INT8); 

    int8_t *data = (int8_t *)vec->data;
    for (size_t i = 0; i < vec->size; i++) {
        data[i] = (int8_t)0;
    }
}

void scalar_add_scalar_i8(const vector_t *vec1, const int scalar, vector_t *result) {
    assert(vec1->type == result->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == result->size);

    int8_t *vec1_data = (int8_t *)vec1->data; 
    int8_t *result_data = (int8_t *)result->data;

    for (int i = 0; i < vec1->size; i++) {
        int intermediate = vec1_data[i] + scalar;
        intermediate = intermediate > 127 ? 127 : (intermediate < -128 ? -128 : intermediate);
        result_data[i] = (int8_t)(intermediate);
    }
}   

void scalar_mul_scalar_shift_i8(const vector_t *vec1, const int val, vector_t *result, const int shift_amount) {
    assert(vec1->type == result->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == result->size);
    assert(shift_amount >= 0 && shift_amount <= 8); // Ensure shift amount is within valid range for int8_t

    int8_t *vec1_data = (int8_t *)vec1->data; 
    int8_t *result_data = (int8_t *)result->data;

    for (int i = 0; i < vec1->size; i++) {
        int intermediate = vec1_data[i] * val;
        intermediate = intermediate >> shift_amount; 
        result_data[i] = (int8_t)(intermediate);
    } 
}   

void scalar_sum_i8(const vector_t *vec1, int32_t* result) {
    assert(vec1->type == DTYPE_INT8);
    assert(vec1->size > 0);

    int8_t *vec1_data = (int8_t *)vec1->data;
    int32_t sum = 0;

    for (int i = 0; i < vec1->size; i++) {
        sum += vec1_data[i];
    }
    *result = sum;
}

void scalar_relu_i8(const vector_t *vec1, vector_t *result, const int multiplier, const int shift_amount) {
    assert(vec1->type == result->type && vec1->type == DTYPE_INT8);
    assert(vec1->size == result->size);
    assert(multiplier >= 0 && multiplier < 128);
    assert(shift_amount >= 0 && shift_amount < 32);

    int8_t *vec1_data = (int8_t *)vec1->data; 
    int8_t *result_data = (int8_t *)result->data;

    for (int i = 0; i < vec1->size; i++) {
        int val = vec1_data[i];
        if (val > 0) {
            result_data[i] = (int8_t)val;
        } else {
            int intermediate = (val * multiplier) >> shift_amount;
            result_data[i] = (int8_t)intermediate;
        }
    }
}