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


## 📊 Performance

We benchmarked a set of vector math operations (add, sub, mul, dot product, sum, and bitwise ops) across `int8`, `int16`, `int32`, and `float32` data types. Runtimes were aggregated from 32 runs on random vectors of length 1–256.  

SIMD (vectorized) execution consistently outperforms scalar execution, with **speedups ranging from ~2× to nearly 10×** depending on the operation and data type.  
- Integer arithmetic (especially 32-bit add/sub) shows the largest gains (≈9–10×).  
- Floating-point operations achieve more modest improvements (≈1.5–4×).  
- Memory-bound ops (copy/fill/zero/ones) also benefit, though with smaller absolute times.  

| Operation        | DType    | SIMD (μs)          | Scalar (μs)           | Speedup (Scalar ÷ SIMD) |
|------------------|----------|--------------------|-----------------------|-------------------------|
| **Saturated Add**| INT8     | 199                | 1227                  | **6.2×**                |
|                  | INT16    | 185                | 1214                  | **6.6×**                |
|                  | INT32    | 193                | 1864                  | **9.7×**                |
|                  | FLOAT32  | 267                | 957                   | **3.6×**                |
| **Saturated Sub**| INT8     | 192                | 1134                  | **5.9×**                |
|                  | INT32    | 184                | 1666                  | **9.1×**                |
|                  | FLOAT32  | 262                | 890                   | **3.4×**                |
| **Mul + shift**  | INT8     | 193                | 1167                  | **6.0×**                |
|                  | INT32    | 588                | 2275                  | **3.9×**                |
|                  | FLOAT32  | 279                | 1057                  | **3.8×**                |
| **Dot Product**  | INT8     | 186                | 923                   | **5.0×**                |
|                  | INT32    | 404                | 815                   | **2.0×**                |
|                  | FLOAT32  | 367                | 583                   | **1.6×**                |
| **Sum**          | INT8     | 147                | 662                   | **4.5×**                |
|                  | INT32    | 159                | 1163                  | **7.3×**                |
|                  | FLOAT32  | 267                | 595                   | **2.2×**                |
| **Bitwise AND**  | INT8     | 186                | 820                   | **4.4×**                |
|                  | INT32    | 207                | 864                   | **4.2×**                |
|                  | FLOAT32  | 203                | 861                   | **4.2×**                |

---


## 📦 Installation
Using esp-idf:
Clone the repository into your project’s `components` directory. 
If your project does not have components directory, create the folder on the top level directory, (i.e same level as 'main' and 'build').
e.g. project-name/components

```bash
git clone https://github.com/zliu43/esp_simd.git components/esp_simd
```

Add esp_simd to the REQUIRES field in the CMakeLists.txt in 'main':
e.g. project-name/main/CMakeLists.txt

```bash
idf_component_register(SRCS "hello_world_main.c"
                       PRIV_REQUIRES spi_flash esp_simd
                       INCLUDE_DIRS "")
```

Using Arduino:
Go to https://github.com/zliu43/esp_simd and download the library as a zip folder.

Go to Sketch > Include Library > Add .ZIP Library...

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

MIT License

Copyright (c) [2025] [Mike Liu]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---
 
