#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "record.h"

/* ========================================================================= *
 * INSERTION SORTS (O(N^2) - Fast Path for Tiny Arrays)                      *
 * ========================================================================= */

// Primitive Insertion Sorts
void uint32_insertion(uint32_t* arr, size_t n);
void int32_insertion(int32_t* arr, size_t n);
void float_insertion(float* arr, size_t n);
void uint64_insertion(uint64_t* arr, size_t n);
void int64_insertion(int64_t* arr, size_t n);
void double_insertion(double* arr, size_t n);

// Record Insertion Sorts
void recu32_insertion(struct RecU32* arr, size_t n);
void reci32_insertion(struct RecI32* arr, size_t n);
void recf32_insertion(struct RecF32* arr, size_t n);
void recu64_insertion(struct RecU64* arr, size_t n);
void reci64_insertion(struct RecI64* arr, size_t n);
void recf64_insertion(struct RecF64* arr, size_t n);

/* ========================================================================= *
 * HEAP SORTS (O(N log N) - In-Place Safety Fallback)                        *
 * ========================================================================= */

// Primitive Heap Sorts
void uint32_heap_sort(uint32_t* arr, size_t n);
void int32_heap_sort(int32_t* arr, size_t n);
void float_heap_sort(float* arr, size_t n);
void uint64_heap_sort(uint64_t* arr, size_t n);
void int64_heap_sort(int64_t* arr, size_t n);
void double_heap_sort(double* arr, size_t n);

// Record Heap Sorts
void recu32_heap_sort(struct RecU32* arr, size_t n);
void reci32_heap_sort(struct RecI32* arr, size_t n);
void recf32_heap_sort(struct RecF32* arr, size_t n);
void recu64_heap_sort(struct RecU64* arr, size_t n);
void reci64_heap_sort(struct RecI64* arr, size_t n);
void recf64_heap_sort(struct RecF64* arr, size_t n);

/* ========================================================================= *
 * INTROSORTS (O(N log N) - Cache-Friendly QuickSort)                        *
 * ========================================================================= */

// Primitive Introsorts
void uint32_introsort(uint32_t* arr, int low, int high, int depth_limit);
void int32_introsort(int32_t* arr, int low, int high, int depth_limit);
void float_introsort(float* arr, int low, int high, int depth_limit);
void uint64_introsort(uint64_t* arr, int low, int high, int depth_limit);
void int64_introsort(int64_t* arr, int low, int high, int depth_limit);
void double_introsort(double* arr, int low, int high, int depth_limit);

// Record Introsorts
void recu32_introsort(struct RecU32* arr, int low, int high, int depth_limit);
void reci32_introsort(struct RecI32* arr, int low, int high, int depth_limit);
void recf32_introsort(struct RecF32* arr, int low, int high, int depth_limit);
void recu64_introsort(struct RecU64* arr, int low, int high, int depth_limit);
void reci64_introsort(struct RecI64* arr, int low, int high, int depth_limit);
void recf64_introsort(struct RecF64* arr, int low, int high, int depth_limit);

/* ========================================================================= *
 * INTROSORT FALLBACK TRIGGERS (Main entry points for O(N log N))            *
 * ========================================================================= */

// Primitive Fallbacks
void uint32_sort_fallback(uint32_t* arr, size_t n);
void int32_sort_fallback(int32_t* arr, size_t n);
void float_sort_fallback(float* arr, size_t n);
void uint64_sort_fallback(uint64_t* arr, size_t n);
void int64_sort_fallback(int64_t* arr, size_t n);
void double_sort_fallback(double* arr, size_t n);

// Record Fallbacks
void recu32_sort_fallback(struct RecU32* arr, size_t n);
void reci32_sort_fallback(struct RecI32* arr, size_t n);
void recf32_sort_fallback(struct RecF32* arr, size_t n);
void recu64_sort_fallback(struct RecU64* arr, size_t n);
void reci64_sort_fallback(struct RecI64* arr, size_t n);
void recf64_sort_fallback(struct RecF64* arr, size_t n);

/* ========================================================================= *
 * EXPLICITLY TYPED UNIVERSAL SORTS (Radix O(N) - Main Entry Points)         *
 * ========================================================================= */

// Primitive Arrays
void universal_sort_uint32(uint32_t* arr, size_t n);
void universal_sort_int32(int32_t* arr, size_t n);
void universal_sort_float(float* arr, size_t n);
void universal_sort_uint64(uint64_t* arr, size_t n);
void universal_sort_int64(int64_t* arr, size_t n);
void universal_sort_double(double* arr, size_t n);

// Struct Arrays (Direct Radix)
void universal_sort_recu32(struct RecU32* arr, size_t n);
void universal_sort_reci32(struct RecI32* arr, size_t n);
void universal_sort_recf32(struct RecF32* arr, size_t n);
void universal_sort_recu64(struct RecU64* arr, size_t n);
void universal_sort_reci64(struct RecI64* arr, size_t n);
void universal_sort_recf64(struct RecF64* arr, size_t n);

#endif // SORT_H
