#ifndef VECTOR_TEST_HELPER_H
#define VECTOR_TEST_HELPER_H

#include "vector.h"
#include "driver/gptimer.h"

#define CANARY_DATA 0xDEADBEEF
#define MAX_SIZE 256
#define TEST_RUNS 64

extern gptimer_handle_t gptimer;
extern gptimer_config_t timer_config;

static inline void timer_init(void) { 
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
    ESP_ERROR_CHECK(gptimer_enable(gptimer)); 
}

static inline void timer_deinit(void){ 
    ESP_ERROR_CHECK(gptimer_disable(gptimer));
    ESP_ERROR_CHECK(gptimer_del_timer(gptimer)); 
}

static inline void timer_start() { 
    ESP_ERROR_CHECK(gptimer_start(gptimer)); 
}  

static inline void timer_end(uint32_t* result_ptr) {
    uint64_t _count; 
    ESP_ERROR_CHECK(gptimer_stop(gptimer)); 
    ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &_count));  
    *(result_ptr) = *(result_ptr) + (uint32_t)_count; 
    ESP_ERROR_CHECK(gptimer_set_raw_count(gptimer, 0)); 
}  
 
void set_rand_seed(void);
void set_seed(int seed);
int rand_scalar_val(dtype type);
float rand_float_val();
vector_t* create_test_vector(const size_t size, const dtype type);
void fill_test_vector(vector_t *vec);
void vector_print(const char* TAG, const vector_t *vec);
bool vector_check_canary(vector_t* vec);
bool vector_assert_eq(vector_t *vec1, vector_t *vec2);
bool float_eq(float a, float b);

#endif  