# esp_simd
<p >
  <b><span style="font-size:1.5em">A high-level C library providing SIMD-accelerated functions and safe data structures for the <em>ESP32-S3</em> microcontroller.</span></b>
</p>

The xTensa LX7 core on the ESP32-S3 includes custom SIMD instructions, but they are not emitted by the compiler and can only be accessed through inline assembly. This makes them hard to use safely, since they come with strict alignment requirements, unusual saturation semantics, and the usual type and memory safety pitfalls of handwritten assembly. esp_simd wraps these instructions in a safe, high-level API that abstracts away those concerns.
 

## ✨ Features

* High-level vector API with SIMD acceleration
* Hand-written branchless ASM functions using zero-overhead loops
* Up to **30× faster** performance on certain tasks
* Type-safe handling of aligned data structures
* Currently supports signed integers (`int8`,`int16`, `int32`) and 32-bit float types, with unsigned types planned
* Future support: **matrix** and **tensor** data structures

---

## 📦 Installation
Using esp-idf:
Clone the repository into your project’s `components` directory:

```bash
git clone https://github.com/zliu43/esp_simd.git components/esp_simd
```
Using Arduino:
Clone the repository into your Arduino `libraries` folder:

```bash
git clone https://github.com/zliu43/esp_simd.git ~/Documents/Arduino/libraries/esp_simd
```
Note: On Windows, the path is typically C:\Users\<YourName>\Documents\Arduino\libraries\
---

## 🚀 Usage Example

```c
#include <stdint.h>
#include <math.h>
#include "vector.h"

#define VEC_LENGTH 512
#define VEC_LSHIFT 9

/**
 * Calculates the mean value, mean power, and standard deviation of a 512 length vector of int16_ts,
 * representing the output of an accelerometer, for use for downstream ML classification
 */

// Initialize vectors 
vector_t* accelerometer_x_data = vector_create(VEC_LENGTH, DTYPE_INT16);
vector_t* accelerometer_y_data = vector_create(VEC_LENGTH, DTYPE_INT16);
vector_t* accelerometer_z_data = vector_create(VEC_LENGTH, DTYPE_INT16);

// Check vector validity (memory allocation, alignment)
assert(vector_ok(accelerometer_x_data) == VECTOR_SUCCESS);
assert(vector_ok(accelerometer_y_data) == VECTOR_SUCCESS);
assert(vector_ok(accelerometer_z_data) == VECTOR_SUCCESS);

/*
    data acquisition
*/

// Computes averages by calculating the sum and performing division using right shift 
int32_t average_x; 
int32_t average_y; 
int32_t average_z;

vec_sum(accelerometer_x_data, &average_x);
vec_sum(accelerometer_y_data, &average_y);
vec_sum(accelerometer_z_data, &average_z);

average_x = average_x >> VEC_LSHIFT;
average_y = average_y >> VEC_LSHIFT;
average_z = average_z >> VEC_LSHIFT;


// Calculates energy by squaring the readings
vector_t* x_energy = vector_create(VEC_LENGTH, DTYPE_INT32);         // DTYPE_INT32 otherwise overflow
assert(vector_ok(x_energy) == VECTOR_SUCCESS);

vec_mul_widen(accelerometer_x_data, accelerometer_x_data, x_energy); // Mul-widen for int16_t * int16_t -> int32_t

int32_t average_x_energy;
vec_sum(x_energy, &average_x_energy);
average_x_energy = average_x_energy >> VEC_LSHIFT; 


// Calculates standard deviation by calculating variance, then using dotp to calculating sum of squares
int32_t sd_x;
vector_t* standard_dev_x = vector_create(VEC_LENGTH, DTYPE_INT16);
assert(vector_ok(standard_dev_x) == VECTOR_SUCCESS);
vec_copy(accelerometer_x_data, standard_dev_x);
vec_add_scalar(standard_dev_x, -average_x);
vec_dotp(standard_dev_x, standard_dev_x, &sd_x);
sd_x = sd_x >> VEC_LSHIFT;
sd_x = (int32_t)(sqrt((float)(sd_x)));


/*
    computation for y and z data ...
*/

vector_destroy(accelerometer_x_data); 
vector_destroy(standard_dev_x);
vector_destroy(x_energy);

int32_t features[] = {average_x, average_y, average_z, average_x_energy, average_y_energy, average_z_energy, sd_x, sd_y, sd_z};

/*
    downstream inference
*/ 

```

---

## ⚙️ Requirements

* ESP32-S3 microcontroller 

---

## 🛠️ Roadmap

* [x] Vector struct with SIMD acceleration
* [ ] Matrix struct
* [ ] Tensor struct
* [ ] Support for unsigned integer data types

---

## 🤝 Contributing

Contributions are welcome!
Feel free to submit issues or pull requests to improve functionality, documentation, or performance.

---

## 📜 License

This project is licensed under the **MIT License**. 

---
 
