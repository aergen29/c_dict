# C-DICT: High-Performance JSON Parser & Dictionary Library

![Language](https://img.shields.io/badge/language-C-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Dependencies](https://img.shields.io/badge/dependencies-zero-lightgrey.svg)

**C-Dict** is a lightweight, zero-dependency C library designed for handling key-value data structures and high-speed JSON serialization/deserialization. 

Unlike traditional parsers that treat everything as generic Linked Lists, **C-Dict automatically optimizes homogeneous arrays into contiguous memory blocks.** This results in significantly lower memory usage and cache-friendly access patterns compared to standard libraries like cJSON.

## Key Features

* **High Performance:** Parses **1 Million integers in ~0.15 seconds**, rivaling industry standards.
* **Smart Memory Optimization:** Automatically detects homogeneous JSON arrays (e.g., `[1, 2, 3]`) and converts them into raw C arrays (`int[]`) instead of heavy Linked Lists.
    * *Result:* Up to **15x less RAM usage** for large numeric datasets.
* **O(N) Complexity:** Implements Tail-Pointer optimization for linear insertion time during parsing.
* **Zero Dependencies:** Built using only standard `stdlib.h`, `stdio.h`, and `string.h`.
* **Type Safety:** `enum`-based dynamic typing system via a polymorphic `variable` struct.

---

## Performance Benchmarks

Benchmarks were performed comparing **C-Dict** against **cJSON (v1.7.18)** on a standard Linux environment.

### Test Case: 1 Million Integers Parsing
Parsing a JSON string containing an array of 1,000,000 integers.

| Library | Operation | Time (sec) | Memory Structure | Est. RAM Usage |
| :--- | :--- | :--- | :--- | :--- |
| **cJSON** | Parse + Sum | 0.1237s | Linked List (Node per item) | ~64 MB |
| **C-Dict** | Parse + Sum | **0.1591s** | **Contiguous Array (`int[]`)** | **~4 MB** |

> **Analysis:** While maintaining competitive parsing speeds (only ~0.03s difference), **C-Dict uses significantly less memory** due to its array optimization strategy. Accessing data in C-Dict is cache-friendly as the data resides in a single contiguous block, whereas Linked List nodes are scattered in heap memory.

---

## Installation & Build

Clone the repository:

```bash
git clone https://github.com/aergen29/c_dict.git
cd c_dict
```

### 1. Build Example Application
To compile and run the usage examples (including `main.c`):

```bash
make
# Builds and runs ./cdict_app
```

### 2. Build as Static Library
To generate the `.a` file for integration into your own projects:

```bash
make lib
# Creates libcdict.a
```

### 3. Integration
Simply copy the `src/` folder and include the headers in your project:

```c
#include "src/dict.h"
#include "src/json.h"
```

---

## Running Comparison Benchmarks (Optional)

The core library is zero-dependency. However, to run the performance comparisons against **cJSON**, you need to fetch the cJSON library and use the test makefile.

1. **Clone cJSON into the project root:**
   ```bash
   git clone https://github.com/DaveGamble/cJSON.git
   ```

2. **Run the Benchmark Build:**
   We provide a separate makefile for testing to keep the main build clean.
   ```bash
   make -f Makefile.test
   ```

3. **Execute:**
   ```bash
   ./benchmark_runner
   ```

---

## Usage Examples

### 1. Dictionary Operations

Easily create and manage key-value pairs with dynamic types.

```c
#include "src/dict.h"

// Create root object
Dict *user = createDict_string("username", "neo");

// Add different types
addDict_int(user, "id", 101);
addDict_bool(user, "active", 1);
addDict_double(user, "score", 95.5);

// Nested Dictionary
Dict *config = createDict_string("theme", "dark");
addDict_dict(user, "settings", config);
```

### 2. JSON Serialization (Stringify)

Convert your Dictionary structure into a JSON string.

```c
#include "src/json.h"

char buffer[4096] = {0};
json_stringify(user, buffer);

printf("%s\n", buffer);
// Output: {"username":"neo", "id":101, "active":true, ...}
```

### 3. JSON Parsing & Optimized Access

The parser automatically handles type detection and optimization.

```c
char *json_data = "{\"data\": [10, 20, 30, 40, 50]}";
Dict *root = json_parse(json_data);

// Accessing the optimized array
variable val = get_val(root, "data");

if (val.type == TYPE_INT_ARR) {
    // Access directly as a C array (Fast & Cache Friendly)
    int *numbers = val.value.int_arr_v;
    int size = val.size;
    
    for(int i = 0; i < size; i++) {
        printf("%d ", numbers[i]); 
    }
}

// Clean up memory
free_dict(root);
```

---

## Project Structure

```
.
├── src/
│   ├── dict.c/h      # Core Dictionary & Memory Management
│   ├── json.c/h      # Recursive Descent Parser & Serializer
│   ├── variable.c/h  # Variant Type System (Polymorphism)
│   └── list.h        # Linked List definitions
├── examples/
│   └── main.c        # Usage examples & Benchmarks
├── Makefile          # Main build configuration (App & Lib)
├── Makefile.test     # Benchmark build configuration (with cJSON)
└── readme.md         # Documentation
```

## Memory Management

Since C-Dict manages dynamic memory allocation (Strings, Arrays, Linked Lists), it provides a robust cleanup function. **Always free your root dictionary** to prevent memory leaks.

```c
free_dict(root); // Recursively frees all keys, values, and nested structures.
```

## API Reference

Comprehensive list of available functions for managing Dictionaries, Arrays, and Variables.

### 1. Dictionary Creation (Root)
Initialize a new dictionary chain. These functions return the pointer to the first node.

```c
// Primitives
Dict *createDict_int(char *key, int value);
Dict *createDict_double(char *key, double value);
Dict *createDict_string(char *key, char *value);
Dict *createDict_bool(char *key, int value); // 1=true, 0=false
Dict *createDict_null(char *key);

// Complex Types
Dict *createDict_dict(char *key, Dict *child); // Nested Dictionary
Dict *createDict_list(char *key, List *list);  // Mixed List

// Generic & Wrapper
Dict *createDict_var(char *key, variable val);
Dict *createDict(DataType type, char *key, void *val);
```

### 2. Adding Items (Insertion)
Append new key-value pairs to an existing dictionary.

```c
// Primitives
Dict *addDict_int(Dict *head, char *key, int value);
Dict *addDict_double(Dict *head, char *key, double value);
Dict *addDict_string(Dict *head, char *key, char *value);
Dict *addDict_bool(Dict *head, char *key, int value);
Dict *addDict_null(Dict *head, char *key);

// Complex Types
Dict *addDict_dict(Dict *head, char *key, Dict *child);
Dict *addDict_list(Dict *head, char *key, List *list);

// Generic & Wrapper
Dict *addDict_var(Dict *head, char *key, variable val);
Dict *addDict(Dict *head, DataType type, char *key, void *val);
```

### 3. Array Operations (Optimized)
Functions to handle contiguous C arrays (e.g., `int[]`) efficiently.

```c
// Create Root with Array
Dict *createDict_int_arr(char *key, int *arr, int size);
Dict *createDict_double_arr(char *key, double *arr, int size);
Dict *createDict_string_arr(char *key, char **arr, int size);
Dict *createDict_bool_arr(char *key, int *arr, int size);
Dict *createDict_null_arr(char *key, int size);

// Add Array to Existing Dict
Dict *addDict_int_arr(Dict *head, char *key, int *arr, int size);
Dict *addDict_double_arr(Dict *head, char *key, double *arr, int size);
Dict *addDict_string_arr(Dict *head, char *key, char **arr, int size);
Dict *addDict_bool_arr(Dict *head, char *key, int *arr, int size);
Dict *addDict_null_arr(Dict *head, char *key, int size);
```

### 4. Management & Utilities
Memory management and data retrieval.

```c
// Memory Cleanup (Crucial)
void free_dict(Dict *head); // Recursively frees the entire structure

// Key Management
int deleteDictKey(Dict **head, char *key); // Returns 1 if deleted, 0 if not found
void setDictKey(Dict *node, char *newKey); // Renames a key

// Retrieval
Dict *getDictVariable(Dict *head, char *key);     // Returns the Dict node
variable getDictVariable_var(Dict *head, char *key); // Returns the value wrapper directly
```

### 5. Low-Level Variable System
Functions to create polymorphic `variable` wrappers manually (used internally or for Lists).

```c
// Scalar Types
variable get_var_int(int value);
variable get_var_double(double value);
variable get_var_string(char *value);
variable get_var_bool(int value);
variable get_var_null();

// Array Types
variable get_var_int_arr(int *value, int size);
variable get_var_double_arr(double *value, int size);
variable get_var_string_arr(char **value, int size);
variable get_var_bool_arr(int *value, int size);
variable get_var_null_arr(int size);

// Structures
variable get_var_dict(Dict *value);
variable get_var_list(List *value);

// Generic
variable get_var(DataType type, void *value);

// List Management
List *create_list(variable value);
void add_var_to_list(List *list, variable value);
```

## License

This project is open-source and available under the MIT License.