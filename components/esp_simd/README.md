 # esp\_simd

A high-level C library providing SIMD-accelerated functions and safe data structures for the **ESP32-S3** microcontroller.
The ESP32-S3 offers SIMD acceleration, but working directly with it can be challenging due to alignment and type-safety issues.
`esp_simd` bridges this gap by exposing intuitive APIs for vectors (and soon matrices/tensors) with built-in SIMD acceleration.

---

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
Clone the repository into your project’s `include` directory:

```bash
git clone https://github.com/zliu43/esp_simd.git include/esp_simd
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
vector_t* accelerometer_x_data = vector_create(2048, DTYPE_INT16);
vector_t* accelerometer_y_data = vector_create(2048, DTYPE_INT16);
vector_t* accelerometer_z_data = vector_create(2048, DTYPE_INT16);

assert(vector_ok(accelerometer_x_data));
assert(vector_ok(accelerometer_y_data));
assert(vector_ok(accelerometer_z_data));

/*
    data acquisition
*/

int32_t average_x; 
int32_t average_y; 
int32_t average_z;
vec_sum(accelerometer_x_data, &average_x);
vec_sum(accelerometer_y_data, &average_y);
vec_sum(accelerometer_z_data, &average_z);

vector_t* x_energy = vector_create(2048, DTYPE_INT32); 
assert(vector_ok(x_energy));

vec_mul_widen(accelerometer_x_data, accelerometer_x_data, x_energy);

int32_t average_x_energy;
vec_sum(x_energy, &average_x_energy);

int32_t max_x;
vec_max(accelerometer_x_data, &max_x);

int32_t min_x;
vec_max(accelerometer_x_data, &min_x);



/*
    ...
*/
vector_destroy(accelerometer_x_data); 
vector_destroy(x_energy);

int32_t* features = {average_x, average_y, average_z, average_x_energy, average_y_energy, average_z_energy, max_x, max_y, max_z
                    ,min_x, min_y, min_z};

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
 