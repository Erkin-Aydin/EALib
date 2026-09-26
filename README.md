# EALib

EALib is an attempt of implementing algorithms and data structures purely in C. It is compiled as a static library with strict compiler warnings and high optimizations (`-O3`, `-flto`), and features parallel processing capabilities utilizing OpenMP. Artificial Intelligence is used in the testing process of the daata structures and algorithmns in this liibrary, as well as for making the implementations faster and optimized. This library is created and maintained as a hobby, so do not expect too much from it. 

## Prerequisites

Before building EALib, ensure your system has the following dependencies installed:

*   **CMake** (version 3.10 or higher)
*   **A C/C++ Compiler** supporting C11 and C++17 (e.g., GCC or Clang)
*   **OpenMP** (Required for parallelized algorithms like the MST benchmark)
*   **Boost C++ Libraries** (Required for the test suite)

## Project Structure

*   `src/` - Core C source files compiled into the static library.
*   `inc/` - Header files for the library.
*   `test/` - C and C++ source files for testing and benchmarking functionality.

## Build Instructions

EALib uses CMake for its build system. An out-of-source build (e.g., inside a `build/` directory) is recommended to keep your workspace clean.

1. Clone the repository and navigate into it:
   ```bash
   git clone https://github.com/Erkin-Aydin/EALib.git
   cd EALib
   ```

2. Create a build directory and configure the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. Compile the library and test executables:
   ```bash
   make
   ```

This will generate the `libEAlib.a` static library along with several test and benchmark executables.

## Running Tests and Benchmarks

After building, you can run the generated test executables directly from your `build/` directory to verify the library's functionality. For instance, for sorting algorithms:

*   **Sorting Algorithms Test:**
    ```bash
    ./test_sort
    ```
    
## Linking EALib to Your Own Projects

To use EALib in your own C or C++ projects, link the generated `libEAlib.a` static library and ensure you include the `inc/` directory in your compiler's include path. If you are utilizing the OpenMP-accelerated functions, remember to link OpenMP (`-fopenmp`) in your project as well.