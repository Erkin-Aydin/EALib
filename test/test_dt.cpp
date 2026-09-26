#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <cstdint>

extern "C" {
    #include "dynamic_table.h"
}

// =========================================================================
// Overloads for std::sort and std::lower_bound on Record Types
// =========================================================================
bool operator<(const RecU32& a, const RecU32& b) { return a.key < b.key; }
bool operator<(const RecI32& a, const RecI32& b) { return a.key < b.key; }
bool operator<(const RecF32& a, const RecF32& b) { return a.key < b.key; }
bool operator<(const RecU64& a, const RecU64& b) { return a.key < b.key; }
bool operator<(const RecI64& a, const RecI64& b) { return a.key < b.key; }
bool operator<(const RecF64& a, const RecF64& b) { return a.key < b.key; }

/* ========================================================================= *
 * 1. Template Traits
 * ========================================================================= */
template<typename T> struct DtTraits;

#define BIND_PRIMITIVE_DT(TYPE, PREFIX) \
template<> struct DtTraits<TYPE> { \
    using DtType = struct PREFIX##_dt; \
    using KeyType = TYPE; \
    static void init(DtType** t) { init_##PREFIX##_dt(t); } \
    static void insert(DtType* t, TYPE& v) { insert_##PREFIX##_dt(t, v); } \
    static void del(DtType* t, TYPE* out) { delete_##PREFIX##_dt(t, out); } \
    static int lin_search(DtType* t, KeyType k, size_t* p) { return linear_search_##PREFIX##_dt(t, k, p); } \
    static int bin_search(DtType* t, KeyType k, size_t* p) { return binary_search_##PREFIX##_dt(t, k, p); } \
    static void free_dt(DtType** t) { free_##PREFIX##_dt(t); } \
    static const char* name() { return #PREFIX; } \
};

#define BIND_RECORD_DT(TYPE, KEY_T, PREFIX) \
template<> struct DtTraits<TYPE> { \
    using DtType = struct PREFIX##_dt; \
    using KeyType = KEY_T; \
    static void init(DtType** t) { init_##PREFIX##_dt(t); } \
    static void insert(DtType* t, TYPE& v) { insert_##PREFIX##_dt(t, &v); } \
    static void del(DtType* t, TYPE* out) { delete_##PREFIX##_dt(t, out); } \
    static int lin_search(DtType* t, KeyType k, size_t* p) { return linear_search_##PREFIX##_dt(t, k, p); } \
    static int bin_search(DtType* t, KeyType k, size_t* p) { return binary_search_##PREFIX##_dt(t, k, p); } \
    static void free_dt(DtType** t) { free_##PREFIX##_dt(t); } \
    static const char* name() { return #PREFIX; } \
};

BIND_PRIMITIVE_DT(int32_t, I32)
BIND_PRIMITIVE_DT(int64_t, I64)
BIND_PRIMITIVE_DT(uint32_t, U32)
BIND_PRIMITIVE_DT(uint64_t, U64)
BIND_PRIMITIVE_DT(float, F32)
BIND_PRIMITIVE_DT(double, F64)

BIND_RECORD_DT(RecI32, int32_t, RecI32)
BIND_RECORD_DT(RecI64, int64_t, RecI64)
BIND_RECORD_DT(RecU32, uint32_t, RecU32)
BIND_RECORD_DT(RecU64, uint64_t, RecU64)
BIND_RECORD_DT(RecF32, float, RecF32)
BIND_RECORD_DT(RecF64, double, RecF64)

/* ========================================================================= *
 * 2. The Benchmark Logic
 * ========================================================================= */
template<typename T>
void run_dt_benchmark(size_t size, bool print_header = false) {
    if (print_header) {
        std::cout << "\n=========================================================================================\n";
        std::cout << "  TYPE: " << std::setw(15) << std::left << DtTraits<T>::name() 
                  << " | Times in Microseconds (us) for " << size << " elements\n";
        std::cout << "=========================================================================================\n";
        std::cout << std::right << std::setw(17) << "Operation" << " | "
                  << std::setw(15) << "std::vector" << " | "
                  << std::setw(15) << "EALib DT" << " | "
                  << "Winner\n";
        std::cout << "-----------------------------------------------------------------------------------------\n";
    }

    std::vector<T> test_data(size);
    std::mt19937 g(1337);

    if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t> || 
                  std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
        std::uniform_int_distribution<int32_t> dist(1, 10000000);
        for (auto& x : test_data) x = dist(g);
    } else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>) {
        std::uniform_real_distribution<double> dist(1.0, 10000000.0);
        for (auto& x : test_data) x = dist(g);
    } else {
        std::uniform_int_distribution<int32_t> dist(1, 10000000);
        for (auto& x : test_data) { x.key = dist(g); x.data = nullptr; }
    }

    // --- Benchmark: Bulk Insertion ---
    std::vector<T> std_vec;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; ++i) std_vec.push_back(test_data[i]);
    auto end = std::chrono::high_resolution_clock::now();
    double vec_insert_us = std::chrono::duration<double, std::micro>(end - start).count();

    typename DtTraits<T>::DtType* c_table = nullptr;
    DtTraits<T>::init(&c_table);
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; ++i) DtTraits<T>::insert(c_table, test_data[i]);
    end = std::chrono::high_resolution_clock::now();
    double dt_insert_us = std::chrono::duration<double, std::micro>(end - start).count();

    std::vector<typename DtTraits<T>::KeyType> search_keys;
    std::uniform_int_distribution<size_t> idx_dist(0, size - 1);
    for(int i = 0; i < 1000; ++i) {
        if constexpr (std::is_same_v<T, typename DtTraits<T>::KeyType>) search_keys.push_back(std_vec[idx_dist(g)]);
        else search_keys.push_back(std_vec[idx_dist(g)].key);
    }

    // --- Benchmark: Linear Search (1000 queries) ---
    start = std::chrono::high_resolution_clock::now();
    volatile size_t found_std = 0;
    for (auto k : search_keys) {
        if constexpr (std::is_same_v<T, typename DtTraits<T>::KeyType>) {
            auto it = std::find(std_vec.begin(), std_vec.end(), k);
            if (it != std_vec.end()) found_std++;
        } else {
            auto it = std::find_if(std_vec.begin(), std_vec.end(), [k](const T& a){ return a.key == k; });
            if (it != std_vec.end()) found_std++;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    double vec_search_us = std::chrono::duration<double, std::micro>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    volatile size_t found_dt = 0;
    size_t pos;
    for (auto k : search_keys) {
        if (DtTraits<T>::lin_search(c_table, k, &pos)) found_dt++;
    }
    end = std::chrono::high_resolution_clock::now();
    double dt_search_us = std::chrono::duration<double, std::micro>(end - start).count();

    // --- Benchmark: Lazy Sort Overhead (1st Query) ---
    start = std::chrono::high_resolution_clock::now();
    std::sort(std_vec.begin(), std_vec.end());
    end = std::chrono::high_resolution_clock::now();
    double vec_sort_us = std::chrono::duration<double, std::micro>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    size_t dummy_pos;
    DtTraits<T>::bin_search(c_table, search_keys[0], &dummy_pos); 
    end = std::chrono::high_resolution_clock::now();
    double dt_sort_us = std::chrono::duration<double, std::micro>(end - start).count();

    // --- Benchmark: Binary Search (1000 queries) ---
    start = std::chrono::high_resolution_clock::now();
    volatile size_t found_std_bin = 0;
    for (auto k : search_keys) {
        if constexpr (std::is_same_v<T, typename DtTraits<T>::KeyType>) {
            auto it = std::lower_bound(std_vec.begin(), std_vec.end(), k);
            if (it != std_vec.end() && *it == k) found_std_bin++;
        } else {
            T dummy; dummy.key = k; dummy.data = nullptr;
            auto it = std::lower_bound(std_vec.begin(), std_vec.end(), dummy);
            if (it != std_vec.end() && it->key == k) found_std_bin++;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    double vec_bin_us = std::chrono::duration<double, std::micro>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    volatile size_t found_dt_bin = 0;
    for (auto k : search_keys) {
        if (DtTraits<T>::bin_search(c_table, k, &pos)) found_dt_bin++;
    }
    end = std::chrono::high_resolution_clock::now();
    double dt_bin_us = std::chrono::duration<double, std::micro>(end - start).count();

    // --- Benchmark: Bulk Deletion ---
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; ++i) std_vec.pop_back();
    end = std::chrono::high_resolution_clock::now();
    double vec_delete_us = std::chrono::duration<double, std::micro>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    T dummy_out;
    for (size_t i = 0; i < size; ++i) DtTraits<T>::del(c_table, &dummy_out);
    end = std::chrono::high_resolution_clock::now();
    double dt_delete_us = std::chrono::duration<double, std::micro>(end - start).count();

    DtTraits<T>::free_dt(&c_table);

    // Print Results
    auto print_row = [](const char* name, double v_time, double c_time) {
        std::cout << std::setw(17) << name << " | "
                  << std::setw(15) << v_time << " | "
                  << std::setw(15) << c_time << " | "
                  << (v_time < c_time ? "std::vector" : "EALib DT") << "\n";
    };

    print_row("Insert (Bulk)", vec_insert_us, dt_insert_us);
    print_row("LinSearch x1000", vec_search_us, dt_search_us);
    print_row("LazySort(1 Query)", vec_sort_us, dt_sort_us);
    print_row("BinSearch x1000", vec_bin_us, dt_bin_us);
    print_row("Delete (Bulk)", vec_delete_us, dt_delete_us);
}

int main() {
    std::cout << "\nStarting Full Dynamic Table Benchmarks (-O3 required)\n";
    size_t test_size = 1000000; // 1 Million elements

    // Primitives
    run_dt_benchmark<int32_t>(test_size, true);
    run_dt_benchmark<int64_t>(test_size, true);
    run_dt_benchmark<uint32_t>(test_size, true);
    run_dt_benchmark<uint64_t>(test_size, true);
    run_dt_benchmark<float>(test_size, true);
    run_dt_benchmark<double>(test_size, true);

    // Records
    run_dt_benchmark<RecI32>(test_size, true);
    run_dt_benchmark<RecI64>(test_size, true);
    run_dt_benchmark<RecU32>(test_size, true);
    run_dt_benchmark<RecU64>(test_size, true);
    run_dt_benchmark<RecF32>(test_size, true);
    run_dt_benchmark<RecF64>(test_size, true);

    std::cout << "\nBenchmarks Complete.\n";
    return 0;
}

