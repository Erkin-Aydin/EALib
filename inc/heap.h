#ifndef HEAP_H
#define HEAP_H

#include "sort.h"
#include "record.h"
#include <stddef.h>
#include <stdlib.h>

/* ========================================================================= *
 * BINARY HEAP DATA STRUCTURES (Primitives)                                  *
 * ========================================================================= */

typedef struct { uint32_t* data; size_t size; size_t capacity; int is_min_heap; } HeapU32;
typedef struct { int32_t* data; size_t size; size_t capacity; int is_min_heap; } HeapI32;
typedef struct { float* data; size_t size; size_t capacity; int is_min_heap; } HeapF32;
typedef struct { uint64_t* data; size_t size; size_t capacity; int is_min_heap; } HeapU64;
typedef struct { int64_t* data; size_t size; size_t capacity; int is_min_heap;} HeapI64;
typedef struct { double* data; size_t size; size_t capacity; int is_min_heap;} HeapF64;

/* ========================================================================= *
 * BINARY HEAP DATA STRUCTURES (Records)                                     *
 * ========================================================================= */

typedef struct { struct RecU32* data; size_t size; size_t capacity; int is_min_heap; } HeapRecU32;
typedef struct { struct RecI32* data; size_t size; size_t capacity; int is_min_heap; } HeapRecI32;
typedef struct { struct RecF32* data; size_t size; size_t capacity; int is_min_heap; } HeapRecF32;
typedef struct { struct RecU64* data; size_t size; size_t capacity; int is_min_heap; } HeapRecU64;
typedef struct { struct RecI64* data; size_t size; size_t capacity; int is_min_heap; } HeapRecI64;
typedef struct { struct RecF64* data; size_t size; size_t capacity; int is_min_heap; } HeapRecF64;

/* ========================================================================= *
 * EXPLICITLY DECLARED HEAP OPERATIONS                                       *
 * ========================================================================= */

/* U32 */
HeapU32* heap_u32_init(size_t initial_capacity, int is_min_heap);
HeapU32* heap_u32_heapify(const uint32_t* arr, size_t n, int is_min_heap);
void heap_u32_free(HeapU32* heap);
int heap_u32_insert(HeapU32* heap, uint32_t item);
int heap_u32_extract(HeapU32* heap, uint32_t* out_item);
int heap_u32_top(const HeapU32* heap, uint32_t* out_item);
void heap_u32_sift_down(HeapU32* heap, size_t idx);

/* I32 */
HeapI32* heap_i32_init(size_t initial_capacity, int is_min_heap);
HeapI32* heap_i32_heapify(const int32_t* arr, size_t n, int is_min_heap);
void heap_i32_free(HeapI32* heap);
int heap_i32_insert(HeapI32* heap, int32_t item);
int heap_i32_extract(HeapI32* heap, int32_t* out_item);
int heap_i32_top(const HeapI32* heap, int32_t* out_item);
void heap_i32_sift_down(HeapI32* heap, size_t idx);

/* F32 */
HeapF32* heap_f32_init(size_t initial_capacity, int is_min_heap);
HeapF32* heap_f32_heapify(const float* arr, size_t n, int is_min_heap);
void heap_f32_free(HeapF32* heap);
int heap_f32_insert(HeapF32* heap, float item);
int heap_f32_extract(HeapF32* heap, float* out_item);
int heap_f32_top(const HeapF32* heap, float* out_item);
void heap_f32_sift_down(HeapF32* heap, size_t idx);

/* U64 */
HeapU64* heap_u64_init(size_t initial_capacity, int is_min_heap);
HeapU64* heap_u64_heapify(const uint64_t* arr, size_t n, int is_min_heap);
void heap_u64_free(HeapU64* heap);
int heap_u64_insert(HeapU64* heap, uint64_t item);
int heap_u64_extract(HeapU64* heap, uint64_t* out_item);
int heap_u64_top(const HeapU64* heap, uint64_t* out_item);
void heap_u64_sift_down(HeapU64* heap, size_t idx);

/* I64 */
HeapI64* heap_i64_init(size_t initial_capacity, int is_min_heap);
HeapI64* heap_i64_heapify(const int64_t* arr, size_t n, int is_min_heap);
void heap_i64_free(HeapI64* heap);
int heap_i64_insert(HeapI64* heap, int64_t item);
int heap_i64_extract(HeapI64* heap, int64_t* out_item);
int heap_i64_top(const HeapI64* heap, int64_t* out_item);
void heap_i64_sift_down(HeapI64* heap, size_t idx);

/* F64 */
HeapF64* heap_f64_init(size_t initial_capacity, int is_min_heap);
HeapF64* heap_f64_heapify(const double* arr, size_t n, int is_min_heap);
void heap_f64_free(HeapF64* heap);
int heap_f64_insert(HeapF64* heap, double item);
int heap_f64_extract(HeapF64* heap, double* out_item);
int heap_f64_top(const HeapF64* heap, double* out_item);
void heap_f64_sift_down(HeapF64* heap, size_t idx);

/* RECU32 */
HeapRecU32* heap_recu32_init(size_t initial_capacity, int is_min_heap);
HeapRecU32* heap_recu32_heapify(const struct RecU32* arr, size_t n, int is_min_heap);
void heap_recu32_free(HeapRecU32* heap);
int heap_recu32_insert(HeapRecU32* heap, struct RecU32 item);
int heap_recu32_extract(HeapRecU32* heap, struct RecU32* out_item);
int heap_recu32_top(const HeapRecU32* heap, struct RecU32* out_item);
void heap_recu32_sift_down(HeapRecU32* heap, size_t idx);

/* RECI32 */
HeapRecI32* heap_reci32_init(size_t initial_capacity, int is_min_heap);
HeapRecI32* heap_reci32_heapify(const struct RecI32* arr, size_t n, int is_min_heap);
void heap_reci32_free(HeapRecI32* heap);
int heap_reci32_insert(HeapRecI32* heap, struct RecI32 item);
int heap_reci32_extract(HeapRecI32* heap, struct RecI32* out_item);
int heap_reci32_top(const HeapRecI32* heap, struct RecI32* out_item);
void heap_reci32_sift_down(HeapRecI32* heap, size_t idx);

/* RECF32 */
HeapRecF32* heap_recf32_init(size_t initial_capacity, int is_min_heap);
HeapRecF32* heap_recf32_heapify(const struct RecF32* arr, size_t n, int is_min_heap);
void heap_recf32_free(HeapRecF32* heap);
int heap_recf32_insert(HeapRecF32* heap, struct RecF32 item);
int heap_recf32_extract(HeapRecF32* heap, struct RecF32* out_item);
int heap_recf32_top(const HeapRecF32* heap, struct RecF32* out_item);
void heap_recf32_sift_down(HeapRecF32* heap, size_t idx);

/* RECU64 */
HeapRecU64* heap_recu64_init(size_t initial_capacity, int is_min_heap);
HeapRecU64* heap_recu64_heapify(const struct RecU64* arr, size_t n, int is_min_heap);
void heap_recu64_free(HeapRecU64* heap);
int heap_recu64_insert(HeapRecU64* heap, struct RecU64 item);
int heap_recu64_extract(HeapRecU64* heap, struct RecU64* out_item);
int heap_recu64_top(const HeapRecU64* heap, struct RecU64* out_item);
void heap_recu64_sift_down(HeapRecU64* heap, size_t idx);

/* RECI64 */
HeapRecI64* heap_reci64_init(size_t initial_capacity, int is_min_heap);
HeapRecI64* heap_reci64_heapify(const struct RecI64* arr, size_t n, int is_min_heap);
void heap_reci64_free(HeapRecI64* heap);
int heap_reci64_insert(HeapRecI64* heap, struct RecI64 item);
int heap_reci64_extract(HeapRecI64* heap, struct RecI64* out_item);
int heap_reci64_top(const HeapRecI64* heap, struct RecI64* out_item);
void heap_reci64_sift_down(HeapRecI64* heap, size_t idx);

/* RECF64 */
HeapRecF64* heap_recf64_init(size_t initial_capacity, int is_min_heap);
HeapRecF64* heap_recf64_heapify(const struct RecF64* arr, size_t n, int is_min_heap);
void heap_recf64_free(HeapRecF64* heap);
int heap_recf64_insert(HeapRecF64* heap, struct RecF64 item);
int heap_recf64_extract(HeapRecF64* heap, struct RecF64* out_item);
int heap_recf64_top(const HeapRecF64* heap, struct RecF64* out_item);
void heap_recf64_sift_down(HeapRecF64* heap, size_t idx);

#endif /* HEAP_H */
