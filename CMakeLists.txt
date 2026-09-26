cmake_minimum_required(VERSION 3.10)
project(EALib C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -flto")

# Find OpenMP and enable it
find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(inc)

file(GLOB_RECURSE EALIB_SOURCES "src/*.c")
add_library(EAlib STATIC ${EALIB_SOURCES})

# Link OpenMP to the library
target_link_libraries(EAlib PUBLIC OpenMP::OpenMP_C)

add_executable(test_sort test/test_sort.cpp)
target_link_libraries(test_sort EAlib)

add_executable(test_dt test/test_dt.cpp)
target_link_libraries(test_dt EAlib)

# add_executable(test_bitset test/test_bitset.cpp)
# target_link_libraries(test_bitset EAlib)


add_executable(test_bitset test/test_bitset.cpp)
target_link_libraries(test_bitset EAlib)

# Build the main test executable and link OpenMP
add_executable(run_mst_bench test/test_mst.c)
target_link_libraries(run_mst_bench EAlib OpenMP::OpenMP_C)
