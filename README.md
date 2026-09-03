# libopenvk

A library for interfacing with openvk REST api via C. Requires curl and jansson to work

# Quick start

main.c:
```c
#include <stdio.h>
#include <libopenvk.h>

int main() {
    openvk_data_t *data;
    openvk_init(&data); // Initialize data struct

    openvk_set_instance(data, "https://api.openvk.org"); // Set instance, in this example we'll use official openvk instance

    printf("Instance version: %s\n", openvk_ovk_version(data)); // Call ovk.version method, get version and print it

    openvk_free(data); // Free the data struct

    return 0;
}
```

CMakeLists.txt:
```
project(example)

cmake_minimum_required(VERSION 3.15)

add_subdirectory(libopenvk)

add_executable(example main.c)

target_link_libraries(example PRIVATE openvk)
```

To compile and run:
```sh
mkdir build
cd build
cmake ..
make
./example
```