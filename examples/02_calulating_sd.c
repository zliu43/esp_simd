#include "vector.h" 

#define VEC_LENGTH 512      // Must be power of 2 for efficient bit-shift division
#define VEC_LSHIFT 9        // log2(512) = 9, used instead of division

void app_main(){
    /**
     * Example: Calculate the standard deviation of a 512-length vector of int16_t values.
     * 
     * Context: The vector represents accelerometer readings that will later be used
     * for machine learning classification tasks.
     * 
     * Optimization Techniques:
     *  1. **Bit-shift division for averaging:** Because the vector length is a power of 2,
     *     we can compute the mean using a right shift instead of integer division.
     *     This is faster and avoids costly division instructions.
     *     - For more general integer division needs, `vec_mul()` supports fixed-point
     *       math by combining multiply and shift in one step.
     * 
     *  2. **Dot product for variance calculation:** By subtracting the mean and then
     *     reusing a dot product (`vec_dotp`) on the adjusted vector, we can compute the
     *     sum of squared differences in one efficient pass.
     */

    // Allocate vector for accelerometer data (int16_t samples)
    vector_t* accelerometer_x_data = vector_create(VEC_LENGTH, DTYPE_INT16); 
    assert(vector_ok(accelerometer_x_data) == VECTOR_SUCCESS); // Check allocation success

    /*
        Normally, sensor data would be filled into accelerometer_x_data here.
    */

    // --- Mean calculation ---
    // Compute the sum of the samples
    int32_t average_x;  
    vec_sum(accelerometer_x_data, &average_x); 

    // Divide by length using bit shift (efficient: sum >> 9 == sum / 512)
    average_x = average_x >> VEC_LSHIFT; 


    // --- Energy calculation (mean of squared values) ---
    // Use int32_t for squared values to avoid overflow (int16_t * int16_t can exceed 16-bit range)
    vector_t* x_energy = vector_create(VEC_LENGTH, DTYPE_INT32);         
    assert(vector_ok(x_energy) == VECTOR_SUCCESS);

    // Compute squared values: element-wise multiplication, widening result to 32 bits
    vec_mul_widen(accelerometer_x_data, accelerometer_x_data, x_energy); 

    // Compute mean energy (sum of squares / 512)
    int32_t average_x_energy;
    vec_sum(x_energy, &average_x_energy);
    average_x_energy = average_x_energy >> VEC_LSHIFT; 


    // --- Standard deviation calculation ---
    // Steps: variance = mean((x - mean)^2), then sqrt(variance)
    int32_t sd_x;

    // Create a copy of the data to shift values by subtracting the mean
    vector_t* standard_dev_x = vector_create(VEC_LENGTH, DTYPE_INT16);
    assert(vector_ok(standard_dev_x) == VECTOR_SUCCESS);
    vec_copy(accelerometer_x_data, standard_dev_x);

    // Subtract mean from each element (x - mean)
    vec_add_scalar(standard_dev_x, -average_x);

    // Compute sum of squared differences via dot product.
    // Squared deviations are much smaller than raw squared values,
    // so overflow is unlikely in 32-bit accumulation.
    vec_dotp(standard_dev_x, standard_dev_x, &sd_x);

    // Divide by N (again using bit shift)
    sd_x = sd_x >> VEC_LSHIFT;

    // Final result: take square root to get standard deviation
    sd_x = (int32_t)(sqrt((float)(sd_x)));
}
