#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <cstdlib>
#include <type_traits>
#include <cstdint>
#include <string>

extern "C" {
    #include "sort.h"
}

// =========================================================================
// 1. Overloads for std::sort (Structs are defined in sort.h)
// =========================================================================
bool operator<(const RecU32& a, const RecU32& b) { return a.key < b.key; }
bool operator<(const RecI32& a, const RecI32& b) { return a.key < b.key; }
bool operator<(const RecF32& a, const RecF32& b) { return a.key < b.key; }
bool operator<(const RecU64& a, const RecU64& b) { return a.key < b.key; }
bool operator<(const RecI64& a, const RecI64& b) { return a.key < b.key; }
bool operator<(const RecF64& a, const RecF64& b) { return a.key < b.key; }

// =========================================================================
// 2. C-Style Comparators (Required specifically for benchmarking C qsort)
// =========================================================================
int comp_u32(const void* a, const void* b)    { uint32_t A = *(const uint32_t*)a; uint32_t B = *(const uint32_t*)b; return (A > B) - (A < B); }
int comp_int(const void* a, const void* b)    { int32_t A = *(const int32_t*)a; int32_t B = *(const int32_t*)b; return (A > B) - (A < B); }
int comp_float(const void* a, const void* b)  { float A = *(const float*)a; float B = *(const float*)b; return (A > B) - (A < B); }
int comp_u64(const void* a, const void* b)    { uint64_t A = *(const uint64_t*)a; uint64_t B = *(const uint64_t*)b; return (A > B) - (A < B); }
int comp_i64(const void* a, const void* b)    { int64_t A = *(const int64_t*)a; int64_t B = *(const int64_t*)b; return (A > B) - (A < B); }
int comp_double(const void* a, const void* b) { double A = *(const double*)a; double B = *(const double*)b; return (A > B) - (A < B); }

int comp_recu32(const void* a, const void* b) { auto A = ((const RecU32*)a)->key; auto B = ((const RecU32*)b)->key; return (A > B) - (A < B); }
int comp_reci32(const void* a, const void* b) { auto A = ((const RecI32*)a)->key; auto B = ((const RecI32*)b)->key; return (A > B) - (A < B); }
int comp_recf32(const void* a, const void* b) { auto A = ((const RecF32*)a)->key; auto B = ((const RecF32*)b)->key; return (A > B) - (A < B); }
int comp_recu64(const void* a, const void* b) { auto A = ((const RecU64*)a)->key; auto B = ((const RecU64*)b)->key; return (A > B) - (A < B); }
int comp_reci64(const void* a, const void* b) { auto A = ((const RecI64*)a)->key; auto B = ((const RecI64*)b)->key; return (A > B) - (A < B); }
int comp_recf64(const void* a, const void* b) { auto A = ((const RecF64*)a)->key; auto B = ((const RecF64*)b)->key; return (A > B) - (A < B); }

// =========================================================================
// 3. Type Traits Helper
// =========================================================================
template<typename T> struct SortTraits;

template<> struct SortTraits<uint32_t> { static auto get_comp() { return comp_u32; } static const char* name() { return "uint32_t"; } };
template<> struct SortTraits<int32_t>  { static auto get_comp() { return comp_int; } static const char* name() { return "int32_t"; } };
template<> struct SortTraits<float>    { static auto get_comp() { return comp_float; } static const char* name() { return "float"; } };
template<> struct SortTraits<uint64_t> { static auto get_comp() { return comp_u64; } static const char* name() { return "uint64_t"; } };
template<> struct SortTraits<int64_t>  { static auto get_comp() { return comp_i64; } static const char* name() { return "int64_t"; } };
template<> struct SortTraits<double>   { static auto get_comp() { return comp_double; } static const char* name() { return "double"; } };

template<> struct SortTraits<RecU32> { static auto get_comp() { return comp_recu32; } static const char* name() { return "Struct (U32)"; } };
template<> struct SortTraits<RecI32> { static auto get_comp() { return comp_reci32; } static const char* name() { return "Struct (I32)"; } };
template<> struct SortTraits<RecF32> { static auto get_comp() { return comp_recf32; } static const char* name() { return "Struct (F32)"; } };
template<> struct SortTraits<RecU64> { static auto get_comp() { return comp_recu64; } static const char* name() { return "Struct (U64)"; } };
template<> struct SortTraits<RecI64> { static auto get_comp() { return comp_reci64; } static const char* name() { return "Struct (I64)"; } };
template<> struct SortTraits<RecF64> { static auto get_comp() { return comp_recf64; } static const char* name() { return "Struct (F64)"; } };

// =========================================================================
// 4. The Universal Benchmark Template
// =========================================================================
template<typename T>
void run_type_benchmark(size_t size, bool print_header = false) {
    if (print_header) {
        std::cout << "\n====================================================================================================================================\n";
        std::cout << "  TYPE: " << std::setw(15) << std::left << SortTraits<T>::name() 
                  << " | Avg times over 15 runs (5 warmup) in Microseconds (us)\n";
        std::cout << "====================================================================================================================================\n";
        std::cout << std::right;
        std::cout << std::setw(10) << "Elements" << " | "
                  << std::setw(11) << "C qsort" << " | "
                  << std::setw(11) << "std::sort" << " | "
                  << std::setw(11) << "Universal" << " | "
                  << std::setw(11) << "Introsort" << " | "
                  << std::setw(11) << "HeapSort" << " | "
                  << std::setw(11) << "Insertion" << " | "
                  << "Ranking (Fastest -> Slowest)\n";
        std::cout << "------------------------------------------------------------------------------------------------------------------------------------\n";
    }

    std::vector<T> original(size);
    std::mt19937 g(1337);

    // Data Generation
    if constexpr (std::is_same_v<T, uint32_t>) {
        std::uniform_int_distribution<uint32_t> dist(0, 2000000);
        for (auto& x : original) x = dist(g);
    }
    else if constexpr (std::is_same_v<T, int32_t>) {
        std::uniform_int_distribution<int32_t> dist(-1000000, 1000000);
        for (auto& x : original) x = dist(g);
    } 
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        std::uniform_real_distribution<T> dist(-1000.0, 1000.0);
        for (auto& x : original) x = dist(g);
    } 
    else if constexpr (std::is_same_v<T, uint64_t>) {
        std::uniform_int_distribution<uint64_t> dist(0, 2000000000ULL);
        for (auto& x : original) x = dist(g);
    }
    else if constexpr (std::is_same_v<T, int64_t>) {
        std::uniform_int_distribution<int64_t> dist(-1000000000LL, 1000000000LL);
        for (auto& x : original) x = dist(g);
    }
    else if constexpr (std::is_same_v<T, RecU32>) {
        std::uniform_int_distribution<uint32_t> dist(0, 2000000);
        for (size_t i = 0; i < size; ++i) { original[i].key = dist(g); original[i].data = (void*)(uintptr_t)i; }
    }
    else if constexpr (std::is_same_v<T, RecI32>) {
        std::uniform_int_distribution<int32_t> dist(-1000000, 1000000);
        for (size_t i = 0; i < size; ++i) { original[i].key = dist(g); original[i].data = (void*)(uintptr_t)i; }
    }
    else if constexpr (std::is_same_v<T, RecF32>) {
        std::uniform_real_distribution<float> dist(-1000.0f, 1000.0f);
        for (size_t i = 0; i < size; ++i) { original[i].key = dist(g); original[i].data = (void*)(uintptr_t)i; }
    }
    else if constexpr (std::is_same_v<T, RecU64>) {
        std::uniform_int_distribution<uint64_t> dist(0, 2000000000ULL);
        for (size_t i = 0; i < size; ++i) { original[i].key = dist(g); original[i].data = (void*)(uintptr_t)i; }
    }
    else if constexpr (std::is_same_v<T, RecI64>) {
        std::uniform_int_distribution<int64_t> dist(-1000000000LL, 1000000000LL);
        for (size_t i = 0; i < size; ++i) { original[i].key = dist(g); original[i].data = (void*)(uintptr_t)i; }
    }
    else if constexpr (std::is_same_v<T, RecF64>) {
        std::uniform_real_distribution<double> dist(-1000.0, 1000.0);
        for (size_t i = 0; i < size; ++i) { original[i].key = dist(g); original[i].data = (void*)(uintptr_t)i; }
    }

    const int TOTAL_RUNS = 15;
    const int WARMUP_RUNS = 5;
    const int MEASURE_RUNS = TOTAL_RUNS - WARMUP_RUNS;

    double q_total = 0, std_total = 0, uni_total = 0, intro_total = 0, heap_total = 0, ins_total = 0;
    
    // Create a single working buffer to avoid continuous allocation overhead
    std::vector<T> work_array(size);

    for (int iter = 0; iter < TOTAL_RUNS; ++iter) {
        
        // 1. C qsort
        std::copy(original.begin(), original.end(), work_array.begin());
        auto start_q = std::chrono::high_resolution_clock::now();
        std::qsort(work_array.data(), size, sizeof(T), SortTraits<T>::get_comp());
        auto end_q = std::chrono::high_resolution_clock::now();
        if (iter >= WARMUP_RUNS) q_total += std::chrono::duration<double, std::micro>(end_q - start_q).count();

        // 2. C++ std::sort
        std::copy(original.begin(), original.end(), work_array.begin());
        auto start_std = std::chrono::high_resolution_clock::now();
        std::sort(work_array.begin(), work_array.end());
        auto end_std = std::chrono::high_resolution_clock::now();
        if (iter >= WARMUP_RUNS) std_total += std::chrono::duration<double, std::micro>(end_std - start_std).count();

        // 3. Universal Sort (Radix)
        std::copy(original.begin(), original.end(), work_array.begin());
        auto start_uni = std::chrono::high_resolution_clock::now();
        if constexpr (std::is_same_v<T, uint32_t>)      universal_sort_uint32(work_array.data(), size);
        else if constexpr (std::is_same_v<T, int32_t>)  universal_sort_int32(work_array.data(), size);
        else if constexpr (std::is_same_v<T, float>)    universal_sort_float(work_array.data(), size);
        else if constexpr (std::is_same_v<T, uint64_t>) universal_sort_uint64(work_array.data(), size);
        else if constexpr (std::is_same_v<T, int64_t>)  universal_sort_int64(work_array.data(), size);
        else if constexpr (std::is_same_v<T, double>)   universal_sort_double(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecU32>)   universal_sort_recu32(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecI32>)   universal_sort_reci32(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecF32>)   universal_sort_recf32(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecU64>)   universal_sort_recu64(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecI64>)   universal_sort_reci64(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecF64>)   universal_sort_recf64(work_array.data(), size);
        auto end_uni = std::chrono::high_resolution_clock::now();
        if (iter >= WARMUP_RUNS) uni_total += std::chrono::duration<double, std::micro>(end_uni - start_uni).count();
        if (iter == TOTAL_RUNS - 1 && !std::is_sorted(work_array.begin(), work_array.end())) { std::cerr << "Universal Sort failed!\n"; return; }

        // 4. Introsort
        std::copy(original.begin(), original.end(), work_array.begin());
        auto start_intro = std::chrono::high_resolution_clock::now();
        if constexpr (std::is_same_v<T, uint32_t>)      uint32_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, int32_t>)  int32_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, float>)    float_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, uint64_t>) uint64_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, int64_t>)  int64_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, double>)   double_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecU32>)   recu32_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecI32>)   reci32_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecF32>)   recf32_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecU64>)   recu64_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecI64>)   reci64_sort_fallback(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecF64>)   recf64_sort_fallback(work_array.data(), size);
        auto end_intro = std::chrono::high_resolution_clock::now();
        if (iter >= WARMUP_RUNS) intro_total += std::chrono::duration<double, std::micro>(end_intro - start_intro).count();
        if (iter == TOTAL_RUNS - 1 && !std::is_sorted(work_array.begin(), work_array.end())) { std::cerr << "Introsort failed!\n"; return; }

        // 5. HeapSort
        std::copy(original.begin(), original.end(), work_array.begin());
        auto start_heap = std::chrono::high_resolution_clock::now();
        if constexpr (std::is_same_v<T, uint32_t>)      uint32_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, int32_t>)  int32_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, float>)    float_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, uint64_t>) uint64_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, int64_t>)  int64_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, double>)   double_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecU32>)   recu32_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecI32>)   reci32_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecF32>)   recf32_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecU64>)   recu64_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecI64>)   reci64_heap_sort(work_array.data(), size);
        else if constexpr (std::is_same_v<T, RecF64>)   recf64_heap_sort(work_array.data(), size);
        auto end_heap = std::chrono::high_resolution_clock::now();
        if (iter >= WARMUP_RUNS) heap_total += std::chrono::duration<double, std::micro>(end_heap - start_heap).count();
        if (iter == TOTAL_RUNS - 1 && !std::is_sorted(work_array.begin(), work_array.end())) { std::cerr << "HeapSort failed!\n"; return; }

        // 6. Insertion Sort (Capped at 100,000 items to prevent freezing)
        if (size <= 100000) {
            std::copy(original.begin(), original.end(), work_array.begin());
            auto start_ins = std::chrono::high_resolution_clock::now();
            if constexpr (std::is_same_v<T, uint32_t>)      uint32_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, int32_t>)  int32_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, float>)    float_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, uint64_t>) uint64_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, int64_t>)  int64_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, double>)   double_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, RecU32>)   recu32_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, RecI32>)   reci32_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, RecF32>)   recf32_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, RecU64>)   recu64_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, RecI64>)   reci64_insertion(work_array.data(), size);
            else if constexpr (std::is_same_v<T, RecF64>)   recf64_insertion(work_array.data(), size);
            auto end_ins = std::chrono::high_resolution_clock::now();
            if (iter >= WARMUP_RUNS) ins_total += std::chrono::duration<double, std::micro>(end_ins - start_ins).count();
            if (iter == TOTAL_RUNS - 1 && !std::is_sorted(work_array.begin(), work_array.end())) { std::cerr << "Insertion Sort failed!\n"; return; }
        }
    }

    // Calculate Averages
    double q_us = q_total / MEASURE_RUNS;
    double std_us = std_total / MEASURE_RUNS;
    double uni_us = uni_total / MEASURE_RUNS;
    double intro_us = intro_total / MEASURE_RUNS;
    double heap_us = heap_total / MEASURE_RUNS;
    double ins_us = size <= 100000 ? (ins_total / MEASURE_RUNS) : 0.0;

    // Calculate Rankings
    std::vector<std::pair<double, std::string>> rankings;
    rankings.push_back({q_us, "qsort"});
    rankings.push_back({std_us, "std"});
    rankings.push_back({uni_us, "Univ"});
    rankings.push_back({intro_us, "Intro"});
    rankings.push_back({heap_us, "Heap"});
    if (size <= 100000) {
        rankings.push_back({ins_us, "Insert"});
    }
    
    // Sort rankings by time (Fastest to Slowest)
    std::sort(rankings.begin(), rankings.end());
    
    std::string ranking_str = "";
    for (size_t i = 0; i < rankings.size(); ++i) {
        ranking_str += rankings[i].second;
        if (i < rankings.size() - 1) ranking_str += " < ";
    }

    // Print Results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(10) << size << " | " 
              << std::setw(11) << q_us << " | " 
              << std::setw(11) << std_us << " | " 
              << std::setw(11) << uni_us << " | "
              << std::setw(11) << intro_us << " | "
              << std::setw(11) << heap_us << " | ";
              
    if (size <= 100000) std::cout << std::setw(11) << ins_us << " | ";
    else std::cout << std::setw(11) << "Skipped" << " | ";
    
    std::cout << ranking_str << "\n";
}

int main() {
    std::cout << "\nStarting Multi-Algorithm Benchmark (Make sure -O3 optimizations are enabled!)\n";
    
    std::vector<size_t> sizes = {10, 100, 1000, 10000, 100000, 1000000};

    // Primitives
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<uint32_t>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<int32_t>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<float>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<uint64_t>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<int64_t>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<double>(sizes[i], i == 0);

    // Structs
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<RecU32>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<RecI32>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<RecF32>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<RecU64>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<RecI64>(sizes[i], i == 0);
    for (size_t i = 0; i < sizes.size(); ++i) run_type_benchmark<RecF64>(sizes[i], i == 0);

    std::cout << "\nBenchmark Complete.\n";
    return 0;
}