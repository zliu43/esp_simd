#include "vector.h"
#include "vector_basic_functions.h" 
#include "vector_test_helper.h"
#include "esp_heap_caps.h"  
#include "esp_log.h"
#include "math.h"
#include <time.h>  

gptimer_handle_t gptimer = NULL;
gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Select the default clock source
    .direction = GPTIMER_COUNT_UP,      // Counting direction is up
    .resolution_hz = 1 * 1000 * 1000,   // Resolution is 1 MHz, i.e., 1 tick equals 1 microsecond
}; 
 
// HELPER FUNCTIONS
 
void set_rand_seed(void){
    int seed = time(NULL);              // One second resolution -> switch to esp_timer for more precision
    srand(seed);
    ESP_LOGI("fill_test_vector", "Seed set: %d", seed);
}

void set_seed(int seed){
    srand(seed);
    ESP_LOGI("fill_test_vector", "Seed set: %d", seed);
}

vector_t* create_test_vector(const size_t size, const dtype type){ 
    vector_t *vec = malloc(sizeof(vector_t));
    if (!vec) { 
        ESP_LOGI("create_test_vector", "Failed to allocate memory");
        return NULL;
    } 
 
    vec->data = heap_caps_aligned_alloc(16, size * sizeof_dtype(type) + 4, MALLOC_CAP_DEFAULT); // Allocate aligned memory for the data array
    if (!vec->data) { 
        free(vec);  
        return NULL;
    }
    void* data_end = vec->data + size * sizeof_dtype(type);
    int32_t* canary_region = data_end;
    canary_region[0] = CANARY_DATA;

    vec->type = type;
    vec->size = size;  
    vec->owns_data = true;                                                                  // Indicates that this vector_t owns the data 
    return vec;                                                                             // Return the created vector_t
}

float rand_float_val(){
    float val = 1.0f;
    val *= (rand() & 0x1 ? -1 : 1);
    val *= rand() % 128;
    return val;
}

void fill_test_vector(vector_t *vec){
    switch (vec->type){
        case (DTYPE_INT8): {
            int8_t* data = vec->data;
            for (int i = 0; i < vec->size; i++){
                uint8_t u = (uint8_t)rand();
                data[i] = (int8_t)u;
            }
            break;
        }
        case (DTYPE_INT16): {
            int16_t* data = vec->data;
            for (int i = 0; i < vec->size; i++){ 
                uint16_t u = (uint16_t)rand();
                data[i] = (int16_t)u; 
            }
            break;
        }
        case (DTYPE_INT32): {
            int32_t* data = vec->data;
            for (int i = 0; i < vec->size; i++){
                int32_t val = (rand() << 16) ^ rand();
                data[i] = val;
            }
            break;
        }
        case (DTYPE_FLOAT32): {
            float* data = vec->data;
            for (int i = 0; i < vec->size; i++){
                data[i] = rand_float_val();
            }
        }
    }
}

void vector_print(const char* TAG, const vector_t* vec){
    switch (vec->type)
    {
        case (DTYPE_INT8):
            int8_t *data = vec->data;
            for (int i = 0; i < vec->size; i++)
            {
                int val = (int)(data[i]);
                ESP_LOGI(TAG, "Print %d", val);
            } 
            break; 
        default:
            break;
    }
}

bool vector_check_canary(vector_t* vec){
    void *data_end = vec->data;
    data_end += vec->size * sizeof_dtype(vec->type);
    uint32_t canary_data =*(uint32_t*)data_end;
    if (canary_data != CANARY_DATA){
        ESP_LOGE("vector_check_canary", "Canary modified: expected %d, found: %d", CANARY_DATA, canary_data);
        return false;
    }
    return true;
}
 
int rand_scalar_val(dtype type){
    switch (type){
        case DTYPE_INT8: { 
            return INT8_MIN + rand() % (INT8_MAX - INT8_MIN + 1); 
        }
        case DTYPE_INT16: { 
            return INT16_MIN + rand() % (INT16_MAX - INT16_MIN + 1); 
        }
        case DTYPE_INT32: {  
            return (int32_t)rand();   
        }
        default:
            assert(false);
    }
}
 
bool float_eq(float a, float b){
    const float abs_tol = 1e-6f;     
    const float rel_tol = 1e-3f;     
 
    if (isnan(a) || isnan(b)) return false;
    if (isinf(a) || isinf(b)) return a == b;

    float diff = fabsf(a - b);

    if (diff <= abs_tol) return true;  

    return diff <= rel_tol * fmaxf(fabsf(a), fabsf(b));
}

bool vector_assert_eq(vector_t *vec1, vector_t *vec2){
    if (vec1->type != vec2->type) { ESP_LOGE("vector_assert_eq", "type mismatch"); return false;}
    if (vec1->size != vec2->size) { ESP_LOGE("vector_assert_eq", "size mismatch"); return false;}
    bool equals_flag = true;
    switch (vec1->type){
        case (DTYPE_INT8): {
            int8_t *vec1_data = (int8_t*)(vec1->data);
            int8_t *vec2_data = (int8_t*)(vec2->data);
            for (int i = 0; i < vec1->size; i++)
            {
                int8_t val1 = vec1_data[i]; 
                int8_t val2 = vec2_data[i];
                if (val1 != val2){
                    ESP_LOGE("vector_assert_eq", "Mismatch found at %d, vec1: %d, vec2 %d", i, (int)val1, (int)val2);
                    equals_flag = false;
                }
            } 
            break;
        }
        case (DTYPE_INT16): {
            int16_t *vec1_data = (int16_t*)(vec1->data);
            int16_t *vec2_data = (int16_t*)(vec2->data);
            for (int i = 0; i < vec1->size; i++)
            {
                int16_t val1 = vec1_data[i]; 
                int16_t val2 = vec2_data[i];
                if (val1 != val2){
                    ESP_LOGE("vector_assert_eq", "Mismatch found at %d, vec1: %d, vec2 %d", i, (int)val1, (int)val2);
                    equals_flag = false;
                }
            } 
            break;
        }
        case (DTYPE_INT32): {
            int32_t *vec1_data = (int32_t*)(vec1->data);
            int32_t *vec2_data = (int32_t*)(vec2->data);
            for (int i = 0; i < vec1->size; i++)
            {
                int32_t val1 = vec1_data[i]; 
                int32_t val2 = vec2_data[i];
                if (val1 != val2){
                    ESP_LOGE("vector_assert_eq", "Mismatch found at %d, vec1: %d, vec2 %d", i, (int)val1, (int)val2);
                    equals_flag = false;
                }
            } 
            break;
        }
        case (DTYPE_FLOAT32): {
            float *vec1_data = (float*)(vec1->data);
            float *vec2_data = (float*)(vec2->data);
            for (int i = 0; i < vec1->size; i++)
            {
                float val1 = vec1_data[i]; 
                float val2 = vec2_data[i];
                if (!float_eq(val1,val2)){
                    ESP_LOGE("vector_assert_eq_f32", "Mismatch found at %d, vec1: %f, vec2 %f", i, val1, val2);
                    equals_flag = false;
                }
            } 
            break;
        }
        default:
            break;
    }
    if (!equals_flag){ return false;}
    return true;
}

