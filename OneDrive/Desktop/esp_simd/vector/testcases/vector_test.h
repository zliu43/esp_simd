#include "vector.h"
#include "vector_functions/vector_functions.h"
#include "scalar_functions/scalar_functions.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "esp_log.h"
#include "esp_timer.h"

int test_binary_vec_to_vec(const char *TAG, 
                           vector_t *vec1, vector_t *vec2, 
                           vector_t *result, vector_t *expected_result, 
                           vector_status_t (*vec_func)(const vector_t *, const vector_t *, vector_t *),
                           void (*scalar_func)(const vector_t *, const vector_t *, vector_t *),
                           unsigned int *vector_time_out, unsigned int *scalar_time_out){

    int fails = 0;

    // Backup data for immutability check
    int8_t *vec1_data_copy = malloc(vec1->size);
    int8_t *vec2_data_copy = malloc(vec2->size);
    if (!vec1_data_copy || !vec2_data_copy) {
        ESP_LOGE(TAG, "Memory allocation failed");
        free(vec1_data_copy);
        free(vec2_data_copy);
        return -1;
    }

    int data_size = vec1->size * sizeof_dtype(vec1->type);
    memcpy(vec1_data_copy, vec1->data, data_size);
    memcpy(vec2_data_copy, vec2->data, data_size);

    // Measure vector function runtime
    int64_t start_vector = esp_timer_get_time();
    if (vec_func(vec1, vec2, result) != VECTOR_SUCCESS) {
        ESP_LOGE(TAG, "Vector function failed");
        free(vec1_data_copy);
        free(vec2_data_copy);
        return -1 * vec1->size;
    }
    int64_t end_vector = esp_timer_get_time();
    int64_t vector_time = end_vector - start_vector;

    // Measure scalar function runtime
    int64_t start_scalar = esp_timer_get_time();
    scalar_func(vec1, vec2, expected_result);
    int64_t end_scalar = esp_timer_get_time();
    int64_t scalar_time = end_scalar - start_scalar;

    // Check for correctness
    int8_t *vec1_data = vec1->data;
    int8_t *vec2_data = vec2->data;
    int8_t *result_data = result->data;
    int8_t *expected_result_data = expected_result->data;

    for (int i = 0; i < vec1->size; i++) {
        if (result_data[i] != expected_result_data[i]) {
            ESP_LOGE(TAG, "Mismatch at index %d: expected %d, got %d (input: %d + %d)",
                     i, (int)expected_result_data[i], (int)result_data[i],
                     (int)vec1_data[i], (int)vec2_data[i]);
            fails++;
        }
    }

    // Check for unintended modification of inputs
    for (int i = 0; i < vec1->size; i++) {
        if (vec1_data[i] != vec1_data_copy[i]) {
            ESP_LOGW(TAG, "vec1 modified at index %d: expected %d, got %d",
                     i, vec1_data_copy[i], vec1_data[i]);
            fails++;
        }
        if (vec2_data[i] != vec2_data_copy[i]) {
            ESP_LOGW(TAG, "vec2 modified at index %d: expected %d, got %d",
                     i, vec2_data_copy[i], vec2_data[i]);
            fails++;
        }
    }

    // aggregate runtime results
    *vector_time_out += vector_time;
    *scalar_time_out += scalar_time;
    //ESP_LOGI(TAG, "Vector function time: %lld us, Scalar function time: %lld us", vector_time, scalar_time);

    // Cleanup
    free(vec1_data_copy);
    free(vec2_data_copy);

    return fails;
}

int test_unary_vec_to_vec(const char *TAG, 
                           vector_t *vec1, 
                           vector_t *result, vector_t *expected_result, 
                           vector_status_t (*vec_func)(const vector_t *, vector_t *),
                           void (*scalar_func)(const vector_t *, vector_t *),
                           unsigned int *vector_time_out, unsigned int *scalar_time_out){

    int fails = 0;

    // Backup data for immutability check
    int8_t *vec1_data_copy = malloc(vec1->size); 
    if (!vec1_data_copy ) {
        ESP_LOGE(TAG, "Memory allocation failed");
        free(vec1_data_copy); 
        return -1;
    }

    int data_size = vec1->size * sizeof_dtype(vec1->type);
    memcpy(vec1_data_copy, vec1->data, data_size); 

    // Measure vector function runtime
    int64_t start_vector = esp_timer_get_time();
    if (vec_func(vec1, result) != VECTOR_SUCCESS) {
        ESP_LOGE(TAG, "Vector function failed");
        free(vec1_data_copy); 
        return -1 * vec1->size;
    }
    int64_t end_vector = esp_timer_get_time();
    int64_t vector_time = end_vector - start_vector;

    // Measure scalar function runtime
    int64_t start_scalar = esp_timer_get_time();
    scalar_func(vec1,  expected_result);
    int64_t end_scalar = esp_timer_get_time();
    int64_t scalar_time = end_scalar - start_scalar;

    // Check for correctness
    int8_t *vec1_data = vec1->data; 
    int8_t *result_data = result->data;
    int8_t *expected_result_data = expected_result->data;

    for (int i = 0; i < vec1->size; i++) {
        if (result_data[i] != expected_result_data[i]) {
            ESP_LOGE(TAG, "Mismatch at index %d: expected %d, got %d (input: %d)",
                     i, (int)expected_result_data[i], (int)result_data[i],
                     (int)vec1_data[i]);
            fails++;
        }
    }

    // Check for unintended modification of inputs
    for (int i = 0; i < vec1->size; i++) {
        if (vec1_data[i] != vec1_data_copy[i]) {
            ESP_LOGW(TAG, "vec1 modified at index %d: expected %d, got %d",
                     i, vec1_data_copy[i], vec1_data[i]);
            fails++;
        } 
    }

    // aggregate runtime results
    *vector_time_out += vector_time;
    *scalar_time_out += scalar_time;
    //ESP_LOGI(TAG, "Vector function time: %lld us, Scalar function time: %lld us", vector_time, scalar_time);

    // Cleanup
    free(vec1_data_copy); 

    return fails;
}

int test_vec_scalar_to_vec(const char *TAG, 
                           vector_t *vec1, 
                           int8_t scalar, 
                           vector_t *result, vector_t *expected_result, 
                           vector_status_t (*vec_func)(const vector_t *, const int8_t, vector_t *),
                           void (*scalar_func)(const vector_t *, const int8_t, vector_t *),
                           unsigned int *vector_time_out, unsigned int *scalar_time_out){

    int fails = 0;

    // Backup data for immutability check
    int8_t *vec1_data_copy = malloc(vec1->size);
    if (!vec1_data_copy) {
        ESP_LOGE(TAG, "Memory allocation failed");
        free(vec1_data_copy);
        return -1;
    }

    int data_size = vec1->size * sizeof_dtype(vec1->type);
    memcpy(vec1_data_copy, vec1->data, data_size);

    // Measure vector function runtime
    int64_t start_vector = esp_timer_get_time();
    if (vec_func(vec1, scalar, result) != VECTOR_SUCCESS) {
        ESP_LOGE(TAG, "Vector function failed");
        free(vec1_data_copy);
        return -1 * vec1->size;
    }
    int64_t end_vector = esp_timer_get_time();
    int64_t vector_time = end_vector - start_vector;

    // Measure scalar function runtime
    int64_t start_scalar = esp_timer_get_time();
    scalar_func(vec1, scalar, expected_result);
    int64_t end_scalar = esp_timer_get_time();
    int64_t scalar_time = end_scalar - start_scalar;

    // Check for correctness
    int8_t *vec1_data = vec1->data;
    int8_t *result_data = result->data;
    int8_t *expected_result_data = expected_result->data;

    for (int i = 0; i < vec1->size; i++) {
        if (result_data[i] != expected_result_data[i]) {
            ESP_LOGE(TAG, "Mismatch at index %d: expected %d, got %d (input: %d + %d)",
                     i, (int)expected_result_data[i], (int)result_data[i],
                     (int)vec1_data[i], (int)scalar);
            fails++;
        }
    }

    // Check for unintended modification of inputs
    for (int i = 0; i < vec1->size; i++) {
        if (vec1_data[i] != vec1_data_copy[i]) {
            ESP_LOGW(TAG, "vec1 modified at index %d: expected %d, got %d",
                     i, vec1_data_copy[i], vec1_data[i]);
            fails++;
        }
    }

    // aggregate runtime results
    *vector_time_out += vector_time;
    *scalar_time_out += scalar_time;
    //ESP_LOGI(TAG, "Vector function time: %lld us, Scalar function time: %lld us", vector_time, scalar_time);

    // Cleanup
    free(vec1_data_copy); 

    return fails;
}
// 2. Vec+Vec,Shift → Vec
int test_binary_vec_to_vec_shift(const char *TAG,
                                 vector_t *vec1, vector_t *vec2,
                                 vector_t *result, vector_t *expected_result,
                                 vector_status_t (*vec_func)(const vector_t *, const vector_t *, vector_t *, int),
                                 void (*scalar_func)(const vector_t *, const vector_t *, vector_t *, int),
                                 int shift,
                                 unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec1->size * sizeof_dtype(vec1->type);
    int8_t *copy1 = malloc(bytes), *copy2 = malloc(bytes);
    if (!copy1 || !copy2) { ESP_LOGE(TAG,"malloc failed"); free(copy1); free(copy2); return -1; }
    memcpy(copy1, vec1->data, bytes);
    memcpy(copy2, vec2->data, bytes);

    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec1, vec2, result, shift) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy1); free(copy2); return -vec1->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(vec1, vec2, expected_result, shift);
    int64_t s1 = esp_timer_get_time();

    int8_t *d1 = vec1->data, *d2 = vec2->data, *r = result->data, *e = expected_result->data;
    for (int i = 0; i < vec1->size; i++) {
        if (r[i] != e[i]) {
            ESP_LOGE(TAG,"Mismatch[%d]: got %d exp %d", i, r[i], e[i]);
            fails++;
        }
        if (d1[i]!=copy1[i]||d2[i]!=copy2[i]) {
            ESP_LOGW(TAG,"Input modified[%d]: %d/%d vs %d/%d", i, d1[i], copy1[i], d2[i], copy2[i]);
            fails++;
        }
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy1); free(copy2);
    return fails;
}

// 3. Vec,Scalar → Vec
int test_unary_vec_to_vec_scalar(const char *TAG,
                                  vector_t *vec1, vector_t *result, vector_t *expected_result,
                                  vector_status_t (*vec_func)(const vector_t *, int, vector_t *),
                                  void (*scalar_func)(const vector_t *, int, vector_t *),
                                  int scalar,
                                  unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec1->size * sizeof_dtype(vec1->type);
    int8_t *copy1 = malloc(bytes);
    if (!copy1) { ESP_LOGE(TAG,"malloc failed"); return -1; }
    memcpy(copy1, vec1->data, bytes);

    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec1, scalar, result) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy1); return -vec1->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(vec1, scalar, expected_result);
    int64_t s1 = esp_timer_get_time();

    int8_t *d1 = vec1->data, *r = result->data, *e = expected_result->data;
    for (int i = 0; i < vec1->size; i++) {
        if (r[i] != e[i]) {
            ESP_LOGE(TAG,"Mismatch[%d]: got %d exp %d", i, r[i], e[i]);
            fails++;
        }
        if (d1[i] != copy1[i]) {
            ESP_LOGW(TAG,"Input modified[%d]: %d vs %d", i, d1[i], copy1[i]);
            fails++;
        }
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy1);
    return fails;
}

// 4. Vec,Scalar,Shift → Vec
int test_unary_vec_to_vec_scalar_shift(const char *TAG,
                                        vector_t *vec1,
                                        vector_t *result, vector_t *expected_result,
                                        vector_status_t (*vec_func)(const vector_t *, int, vector_t *, int),
                                        void (*scalar_func)(const vector_t *, int, vector_t *, int),
                                        int scalar, int shift,
                                        unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec1->size * sizeof_dtype(vec1->type);
    int8_t *copy1 = malloc(bytes);
    if (!copy1) { ESP_LOGE(TAG,"malloc failed"); return -1; }
    memcpy(copy1, vec1->data, bytes);

    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec1, scalar, result, shift) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy1); return -vec1->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(vec1, scalar, expected_result, shift);
    int64_t s1 = esp_timer_get_time();

    int8_t *d1 = vec1->data, *r = result->data, *e = expected_result->data;
    for (int i = 0; i < vec1->size; i++) {
        if (r[i] != e[i]) {
            ESP_LOGE(TAG,"Mismatch[%d]: got %d exp %d", i, r[i], e[i]);
            fails++;
        }
        if (d1[i] != copy1[i]) {
            ESP_LOGW(TAG,"Input modified[%d]: %d vs %d", i, d1[i], copy1[i]);
            fails++;
        }
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy1);
    return fails;
}

// 5. Vec → scalar
int test_unary_vec_to_scalar(const char *TAG,
                              vector_t *vec1,
                              int32_t *got, int32_t *want,
                              vector_status_t (*vec_func)(const vector_t *, int32_t *),
                              void (*scalar_func)(const vector_t *, int32_t *),
                              unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec1->size * sizeof_dtype(vec1->type);
    int8_t *copy1 = malloc(bytes);
    if (!copy1) { ESP_LOGE(TAG,"malloc failed"); return -1; }
    memcpy(copy1, vec1->data, bytes);

    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec1, got) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy1); return -vec1->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(vec1, want);
    int64_t s1 = esp_timer_get_time();

    if (*got != *want) {
        ESP_LOGE(TAG,"Scalar mismatch: got %d exp %d", (int)*got, (int)*want);
        fails++;
    }
    // immutability
    for (int i = 0; i < vec1->size; i++) {
        if (((int8_t*)vec1->data)[i] != copy1[i]) {
            ESP_LOGW(TAG,"Input modified[%d]", i);
            fails++;
        }
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy1);
    return fails;
}

// 6. Vec+Vec → scalar
int test_binary_vec_to_scalar(const char *TAG,
                               vector_t *vec1, vector_t *vec2,
                               int32_t *got, int32_t *want,
                               vector_status_t (*vec_func)(const vector_t *, const vector_t *, int32_t *),
                               void (*scalar_func)(const vector_t *, const vector_t *, int32_t *),
                               unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec1->size * sizeof_dtype(vec1->type);
    int8_t *copy1 = malloc(bytes), *copy2 = malloc(bytes);
    if (!copy1||!copy2) { ESP_LOGE(TAG,"malloc failed"); free(copy1); free(copy2); return -1; }
    memcpy(copy1, vec1->data, bytes);
    memcpy(copy2, vec2->data, bytes);

    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec1, vec2, got) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy1); free(copy2); return -vec1->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(vec1, vec2, want);
    int64_t s1 = esp_timer_get_time();

    if (*got != *want) {
        ESP_LOGE(TAG,"Scalar mismatch: got %d exp %d", (int)*got, (int)*want);
        fails++;
    }
    for (int i = 0; i < vec1->size; i++) {
        if (((int8_t*)vec1->data)[i] != copy1[i] ||
            ((int8_t*)vec2->data)[i] != copy2[i]) {
            ESP_LOGW(TAG,"Input modified[%d]", i);
            fails++;
        }
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy1); free(copy2);
    return fails;
}

// 7. Vec → Vec (no extra)
int test_unary_vec_to_vec_basic(const char *TAG,
                                 vector_t *vec1, vector_t *result, vector_t *expected_result,
                                 vector_status_t (*vec_func)(const vector_t *, vector_t *),
                                 void (*scalar_func)(const vector_t *, vector_t *),
                                 unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    // identical to your existing test_unary_vec_to_vec
    return test_unary_vec_to_vec(TAG, vec1, result, expected_result,
                                 vec_func, scalar_func,
                                 vector_time_out, scalar_time_out);
}

// 8. Vec→Vec,param
int test_unary_vec_to_vec_param(const char *TAG,
                                 vector_t *vec1, vector_t *result, vector_t *expected_result,
                                 vector_status_t (*vec_func)(const vector_t *, vector_t *, int),
                                 void (*scalar_func)(const vector_t *, vector_t *, int),
                                 int param,
                                 unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec1->size * sizeof_dtype(vec1->type);
    int8_t *copy1 = malloc(bytes);
    if (!copy1) { ESP_LOGE(TAG,"malloc failed"); return -1; }
    memcpy(copy1, vec1->data, bytes);

    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec1, result, param) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy1); return -vec1->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(vec1, expected_result, param);
    int64_t s1 = esp_timer_get_time();

    int8_t *d1 = vec1->data, *r = result->data, *e = expected_result->data;
    for (int i = 0; i < vec1->size; i++) {
        if (r[i] != e[i]) { ESP_LOGE(TAG,"Mismatch[%d]: got %d exp %d", i, (int)r[i], (int)e[i]); fails++; }
        if (d1[i]!=copy1[i]) { ESP_LOGW(TAG,"Modified[%d]",i); fails++; }
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy1);
    return fails;
}

// 9. Vec→Vec,mul,shift
int test_unary_vec_to_vec_mul_shift(const char *TAG,
                                     vector_t *vec1, vector_t *result, vector_t *expected_result,
                                     vector_status_t (*vec_func)(const vector_t *, vector_t *, int, int),
                                     void (*scalar_func)(const vector_t *, vector_t *, int, int),
                                     int mul, int shift,
                                     unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec1->size * sizeof_dtype(vec1->type);
    int8_t *copy1 = malloc(bytes);
    if (!copy1) { ESP_LOGE(TAG,"malloc failed"); return -1; }
    memcpy(copy1, vec1->data, bytes);

    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec1, result, mul, shift) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy1); return -vec1->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(vec1, expected_result, mul, shift);
    int64_t s1 = esp_timer_get_time();

    int8_t *d1 = vec1->data, *r = result->data, *e = expected_result->data;
    for (int i = 0; i < vec1->size; i++) {
        if (r[i]!=e[i]) { ESP_LOGE(TAG,"Mismatch[%d]",i); fails++; }
        if (d1[i]!=copy1[i]) { ESP_LOGW(TAG,"Modified[%d]",i); fails++; }
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy1);
    return fails;
}

// 10. MAC: Vec,acc→scalar
int test_vec_mac(const char *TAG,
                 vector_t *vec1,
                 int32_t *acc_vec, int32_t *acc_scl,
                 vector_status_t (*vec_func)(const vector_t *, int32_t *, int),
                 void (*scalar_func)(const vector_t *, int32_t *, int),
                 int mul,
                 unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec1->size * sizeof_dtype(vec1->type);
    int8_t *copy1 = malloc(bytes);
    if (!copy1) { ESP_LOGE(TAG,"malloc failed"); return -1; }
    memcpy(copy1, vec1->data, bytes);

    *acc_vec = 0; *acc_scl = 0;
    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec1, acc_vec, mul) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy1); return -vec1->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(vec1, acc_scl, mul);
    int64_t s1 = esp_timer_get_time();

    if (*acc_vec != *acc_scl) {
        ESP_LOGE(TAG,"MAC mismatch: %d vs %d", (int)*acc_vec, (int)*acc_scl);
        fails++;
    }
    for (int i = 0; i < vec1->size; i++) {
        if (((int8_t*)vec1->data)[i] != copy1[i]) {
            ESP_LOGW(TAG,"Modified[%d]",i);
            fails++;
        }
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy1);
    return fails;
}

// 11. zeros
int test_vec_zero(const char *TAG,
                  vector_t *vec_out, vector_t *expected_zero,
                  vector_status_t (*vec_func)(vector_t *),
                  void (*scalar_func)(vector_t *),
                  unsigned int *vector_time_out, unsigned int *scalar_time_out)
{
    int fails = 0;
    size_t bytes = vec_out->size * sizeof_dtype(vec_out->type);
    int8_t *copy_out = malloc(bytes);
    if (!copy_out) { ESP_LOGE(TAG,"malloc failed"); return -1; }
    memcpy(copy_out, vec_out->data, bytes);

    int64_t t0 = esp_timer_get_time();
    if (vec_func(vec_out) != VECTOR_SUCCESS) { ESP_LOGE(TAG,"vec_fn failed"); free(copy_out); return -vec_out->size; }
    int64_t t1 = esp_timer_get_time();

    int64_t s0 = esp_timer_get_time();
    scalar_func(expected_zero);
    int64_t s1 = esp_timer_get_time();

    int8_t *d = vec_out->data, *e = expected_zero->data;
    for (int i = 0; i < vec_out->size; i++) {
        if (d[i] != e[i]) {
            ESP_LOGE(TAG,"Zero mismatch[%d]: %d vs %d", i, (int)d[i], (int)e[i]);
            fails++;
        }
        // we allow modifying the output, but nothing else exists
    }

    *vector_time_out += (unsigned)(t1 - t0);
    *scalar_time_out += (unsigned)(s1 - s0);
    free(copy_out);
    return fails;
}
