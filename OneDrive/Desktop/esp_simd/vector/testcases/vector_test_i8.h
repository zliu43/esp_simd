#include "vector.h"
#include "vector_test.h"
#include "vector_functions/vector_functions.h"
#include "scalar_functions/scalar_functions.h"
#include <stdlib.h>
#include <time.h>
#include "esp_log.h"

#define TEST_SIZE 32

// 1. Vec+Vec → Vec
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, const vector_t*, vector_t*);
    void (*scalar_func)(const vector_t*, const vector_t*, vector_t*);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} binary_vector_test_case_t;

// 2. Vec+Vec,Shift → Vec
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, const vector_t*, vector_t*, int);
    int shift;
    void (*scalar_func)(const vector_t*, const vector_t*, vector_t*, int);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} binary_vector_shift_test_case_t;

// 3. Vec,Scalar → Vec
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, int, vector_t*);
    int scalar;
    void (*scalar_func)(const vector_t*, int, vector_t*);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} unary_vector_scalar_test_case_t;

// 4. Vec,Scalar,Shift → Vec
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, int, vector_t*, int);
    int scalar;
    int shift;
    void (*scalar_func)(const vector_t*, int, vector_t*, int);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} unary_vector_scalar_shift_test_case_t;

// 5. Vec → scalar
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, int32_t*);
    void (*scalar_func)(const vector_t*, int32_t*);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} unary_vector_to_scalar_test_case_t;

// 6. Vec+Vec → scalar
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, const vector_t*, int32_t*);
    void (*scalar_func)(const vector_t*, const vector_t*, int32_t*);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} binary_vector_to_scalar_test_case_t;

// 7. Vec → Vec (no extra)
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, vector_t*);
    void (*scalar_func)(const vector_t*, vector_t*);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} unary_vector_test_case_t;

// 8. Vec → Vec,Param → Vec (e.g. ceil/floor)
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, vector_t*, int);
    int param;
    void (*scalar_func)(const vector_t*, vector_t*, int);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} unary_vector_param_test_case_t;

// 9. Vec → Vec,Mul,Shift → Vec (ReLU)
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, vector_t*, int, int);
    int mul;
    int shift;
    void (*scalar_func)(const vector_t*, vector_t*, int, int);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} unary_vector_mul_shift_test_case_t;

// 10. Vec,Acc→ scalar (MAC)
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(const vector_t*, int32_t*, int);
    int mul;
    void (*scalar_func)(const vector_t*, int32_t*, int);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} vector_mac_test_case_t;

// 11. zeros
typedef struct {
    const char *name;
    vector_status_t (*vec_func)(vector_t*);
    void (*scalar_func)(vector_t*);
    unsigned int total_vec_time;
    unsigned int total_scalar_time;
} vector_zero_test_case_t;


// --- test‐case arrays ---

binary_vector_test_case_t binary_tests[] = {
    { "vec_add", vec_add, scalar_add_i8, 0, 0 },
    { "vec_sub", vec_sub, scalar_sub_i8, 0, 0 },
    //{ "vec_mul", vec_mul, scalar_mul_i8, 0, 0 },
    { "vec_gt",  vec_gt,  scalar_compare_gt_i8, 0, 0 },
    { "vec_lt",  vec_lt,  scalar_compare_lt_i8, 0, 0 },
    { "vec_eq",  vec_eq,  scalar_compare_eq_i8, 0, 0 },
};
size_t num_binary = sizeof(binary_tests)/sizeof(*binary_tests);

binary_vector_shift_test_case_t binary_shift_tests[] = {
    { "vec_mul_shift", vec_mul_shift, 3, scalar_mul_shift_i8, 0, 0 },
};
size_t num_binary_shift = sizeof(binary_shift_tests)/sizeof(*binary_shift_tests);

unary_vector_scalar_test_case_t unary_scalar_tests[] = {
    { "vec_add_scalar", vec_add_scalar,  5,  scalar_add_scalar_i8, 0, 0 },
};
size_t num_unary_scalar = sizeof(unary_scalar_tests)/sizeof(*unary_scalar_tests);

unary_vector_scalar_shift_test_case_t unary_scalar_shift_tests[] = {
    { "vec_mul_scalar_shift", vec_mul_scalar_shift, 2, 4, scalar_mul_scalar_shift_i8, 0, 0 },
};
size_t num_unary_scalar_shift = sizeof(unary_scalar_shift_tests)/sizeof(*unary_scalar_shift_tests);

unary_vector_to_scalar_test_case_t unary_to_scalar_tests[] = {
    { "vec_sum",  vec_sum,   scalar_sum_i8,   0, 0 },
};
size_t num_unary_to_scalar = sizeof(unary_to_scalar_tests)/sizeof(*unary_to_scalar_tests);

binary_vector_to_scalar_test_case_t binary_to_scalar_tests[] = {
    { "vec_dotp", vec_dotp, scalar_dotp_i8, 0, 0 },
};
size_t num_binary_to_scalar = sizeof(binary_to_scalar_tests)/sizeof(*binary_to_scalar_tests);

unary_vector_test_case_t unary_tests[] = {
    { "vec_abs", vec_abs, scalar_abs_i8, 0, 0 },
    { "vec_neg", vec_neg, scalar_neg_i8, 0, 0 },
};
size_t num_unary = sizeof(unary_tests)/sizeof(*unary_tests);

unary_vector_param_test_case_t unary_param_tests[] = {
    { "vec_ceil",  vec_ceil,  10, scalar_ceil_i8,  0, 0 },
    { "vec_floor", vec_floor, -10, scalar_floor_i8,  0, 0 },
};
size_t num_unary_param = sizeof(unary_param_tests)/sizeof(*unary_param_tests);

unary_vector_mul_shift_test_case_t relu_tests[] = {
    { "vec_relu", vec_relu, 2, 1, scalar_relu_i8, 0, 0 },
};
size_t num_relu = sizeof(relu_tests)/sizeof(*relu_tests);

vector_mac_test_case_t mac_tests[] = {
    { "vec_mac", vec_mac, 3, scalar_mac_i8, 0, 0 },
};
size_t num_mac = sizeof(mac_tests)/sizeof(*mac_tests);

vector_zero_test_case_t zero_tests[] = {
    { "vec_zeros", vec_zeros, scalar_zero_i8, 0, 0 },
};
size_t num_zero = sizeof(zero_tests)/sizeof(*zero_tests);


// --- the unified test runner ---

void test_vector_i8(void) {
    srand(time(NULL));
    ESP_LOGI("vector_test_i8", "Running all %d patterns…", TEST_SIZE);

    int size = rand() % 256 + 1;
    vector_t *v1 = vector_create(size, DTYPE_INT8);
    vector_t *v2 = vector_create(size, DTYPE_INT8);
    vector_t *out = vector_create(size, DTYPE_INT8);
    vector_t *exp = vector_create(size, DTYPE_INT8);
    assert(v1 && v2 && out && exp);

    int fails = 0;
    for (int t = 0; t < TEST_SIZE; t++) {
        // fill inputs
        for (int i = 0; i < size; i++) {
            ((int8_t*)v1->data)[i] = rand() % 256 - 128;
            ((int8_t*)v2->data)[i] = rand() % 256 - 128;
        }

        // 1. Vec+Vec → Vec
        for (size_t k = 0; k < num_binary; k++) {
            fails += test_binary_vec_to_vec(
                binary_tests[k].name,
                v1, v2, out, exp,
                binary_tests[k].vec_func,
                binary_tests[k].scalar_func,
                &binary_tests[k].total_vec_time,
                &binary_tests[k].total_scalar_time
            );
        }

        // 2. Vec+Vec,Shift → Vec
        for (size_t k = 0; k < num_binary_shift; k++) {
            fails += test_binary_vec_to_vec_shift(
                binary_shift_tests[k].name,
                v1, v2, out, exp,
                binary_shift_tests[k].vec_func,
                binary_shift_tests[k].scalar_func,
                binary_shift_tests[k].shift,
                &binary_shift_tests[k].total_vec_time,
                &binary_shift_tests[k].total_scalar_time
            );
        }

        // 3. Vec,Scalar → Vec
        for (size_t k = 0; k < num_unary_scalar; k++) {
            fails += test_unary_vec_to_vec_scalar(
                unary_scalar_tests[k].name,
                v1, out, exp,
                unary_scalar_tests[k].vec_func,
                unary_scalar_tests[k].scalar_func,
                unary_scalar_tests[k].scalar,
                &unary_scalar_tests[k].total_vec_time,
                &unary_scalar_tests[k].total_scalar_time
            );
        }

        // 4. Vec,Scalar,Shift → Vec
        for (size_t k = 0; k < num_unary_scalar_shift; k++) {
            fails += test_unary_vec_to_vec_scalar_shift(
                unary_scalar_shift_tests[k].name,
                v1, out, exp,
                unary_scalar_shift_tests[k].vec_func,
                unary_scalar_shift_tests[k].scalar_func,
                unary_scalar_shift_tests[k].scalar,
                unary_scalar_shift_tests[k].shift,
                &unary_scalar_shift_tests[k].total_vec_time,
                &unary_scalar_shift_tests[k].total_scalar_time
            );
        }

        // 5. Vec → scalar
        for (size_t k = 0; k < num_unary_to_scalar; k++) {
            int32_t got, want;
            fails += test_unary_vec_to_scalar(
                unary_to_scalar_tests[k].name,
                v1,
                &got, &want,
                unary_to_scalar_tests[k].vec_func,
                unary_to_scalar_tests[k].scalar_func,
                &unary_to_scalar_tests[k].total_vec_time,
                &unary_to_scalar_tests[k].total_scalar_time
            );
        }

        // 6. Vec+Vec → scalar
        for (size_t k = 0; k < num_binary_to_scalar; k++) {
            int32_t got, want;
            fails += test_binary_vec_to_scalar(
                binary_to_scalar_tests[k].name,
                v1, v2,
                &got, &want,
                binary_to_scalar_tests[k].vec_func,
                binary_to_scalar_tests[k].scalar_func,
                &binary_to_scalar_tests[k].total_vec_time,
                &binary_to_scalar_tests[k].total_scalar_time
            );
        }

        // 7. Vec → Vec (abs/neg)
        for (size_t k = 0; k < num_unary; k++) {
            fails += test_unary_vec_to_vec(
                unary_tests[k].name,
                v1, out, exp,
                unary_tests[k].vec_func,
                unary_tests[k].scalar_func,
                &unary_tests[k].total_vec_time,
                &unary_tests[k].total_scalar_time
            );
        }

        // 8. Vec→Vec,param (ceil/floor)
        for (size_t k = 0; k < num_unary_param; k++) {
            fails += test_unary_vec_to_vec_param(
                unary_param_tests[k].name,
                v1, out, exp,
                unary_param_tests[k].vec_func,
                unary_param_tests[k].scalar_func,
                unary_param_tests[k].param,
                &unary_param_tests[k].total_vec_time,
                &unary_param_tests[k].total_scalar_time
            );
        }

        // 9. Vec→Vec,mul,shift (ReLU)
        for (size_t k = 0; k < num_relu; k++) {
            fails += test_unary_vec_to_vec_mul_shift(
                relu_tests[k].name,
                v1, out, exp,
                relu_tests[k].vec_func,
                relu_tests[k].scalar_func,
                relu_tests[k].mul, relu_tests[k].shift,
                &relu_tests[k].total_vec_time,
                &relu_tests[k].total_scalar_time
            );
        }

        // 10. MAC: Vec,acc→scalar
        for (size_t k = 0; k < num_mac; k++) {
            int32_t acc_vec = 0, acc_scl = 0;
            fails += test_vec_mac(
                mac_tests[k].name,
                v1,
                &acc_vec, &acc_scl,
                mac_tests[k].vec_func,
                mac_tests[k].scalar_func,
                mac_tests[k].mul,
                &mac_tests[k].total_vec_time,
                &mac_tests[k].total_scalar_time
            );
        }

        // 11. zeros
        for (size_t k = 0; k < num_zero; k++) {
            fails += test_vec_zero(
                zero_tests[k].name,
                out, // we reuse ‘out’ as the target
                exp,
                zero_tests[k].vec_func,
                zero_tests[k].scalar_func,
                &zero_tests[k].total_vec_time,
                &zero_tests[k].total_scalar_time
            );
        }
    }

    ESP_LOGI("vector_test_i8", "Total fails: %d", fails);

    // …print out timing summaries for each pattern array…
    // (just repeat your ESP_LOGI loop over each test‐case array)

    vector_destroy(v1);
    vector_destroy(v2);
    vector_destroy(out);
    vector_destroy(exp);
}
