#include "sort.h"
#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


static inline int fast_log2(size_t n) 
{
#if defined(__GNUC__) || defined(__clang__)
    return (sizeof(size_t) * 8 - 1) - __builtin_clzll(n);
#else
    int depth = 0;
    while (n >>= 1) depth++;
    return depth;
#endif
}

static void uint32_swap(uint32_t* a, uint32_t* b) 
{ 
    uint32_t tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void int32_swap(int32_t* a, int32_t* b) 
{ 
    int32_t tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void float_swap(float* a, float* b) 
{ 
    float tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void uint64_swap(uint64_t* a, uint64_t* b) 
{ 
    uint64_t tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void int64_swap(int64_t* a, int64_t* b) 
{ 
    int64_t tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void double_swap(double* a, double* b) 
{ 
    double tmp = *a; 
    *a = *b; 
    *b = tmp; 
}

// Record Swaps
static void recu32_swap(struct RecU32* a, struct RecU32* b) 
{ 
    struct RecU32 tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void reci32_swap(struct RecI32* a, struct RecI32* b) 
{ 
    struct RecI32 tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void recf32_swap(struct RecF32* a, struct RecF32* b) 
{ 
    struct RecF32 tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void recu64_swap(struct RecU64* a, struct RecU64* b) 
{ 
    struct RecU64 tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void reci64_swap(struct RecI64* a, struct RecI64* b) 
{ 
    struct RecI64 tmp = *a; 
    *a = *b; 
    *b = tmp; 
}
static void recf64_swap(struct RecF64* a, struct RecF64* b) 
{ 
    struct RecF64 tmp = *a; 
    *a = *b; 
    *b = tmp; 
}

void uint32_insertion(uint32_t* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        uint32_t key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void int32_insertion(int32_t* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        int32_t key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void float_insertion(float* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        float key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void uint64_insertion(uint64_t* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        uint64_t key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void int64_insertion(int64_t* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        int64_t key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void double_insertion(double* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        double key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}

// Record Insertion Sorts
void recu32_insertion(struct RecU32* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        struct RecU32 key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j].key > key.key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void reci32_insertion(struct RecI32* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        struct RecI32 key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j].key > key.key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void recf32_insertion(struct RecF32* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        struct RecF32 key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j].key > key.key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void recu64_insertion(struct RecU64* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        struct RecU64 key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j].key > key.key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void reci64_insertion(struct RecI64* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        struct RecI64 key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j].key > key.key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}
void recf64_insertion(struct RecF64* arr, size_t n) 
{
    for (size_t i = 1; i < n; i++) 
    {
        struct RecF64 key = arr[i]; 
        int j = (int)i - 1;
        while (j >= 0 && arr[j].key > key.key) 
        { 
            arr[j + 1] = arr[j]; 
            j--; 
        }
        arr[j + 1] = key;
    }
}

void uint32_heap_sort(uint32_t* arr, size_t n) 
{
    if (n < 2) return;
    HeapU32 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_u32_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        uint32_t tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_u32_sift_down(&h, 0);
    }
}
void int32_heap_sort(int32_t* arr, size_t n) 
{
    if (n < 2) return;
    HeapI32 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_i32_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        int32_t tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_i32_sift_down(&h, 0);
    }
}
void float_heap_sort(float* arr, size_t n) 
{
    if (n < 2) return;
    HeapF32 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_f32_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        float tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_f32_sift_down(&h, 0);
    }
}
void uint64_heap_sort(uint64_t* arr, size_t n) 
{
    if (n < 2) return;
    HeapU64 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_u64_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        uint64_t tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_u64_sift_down(&h, 0);
    }
}
void int64_heap_sort(int64_t* arr, size_t n) 
{
    if (n < 2) return;
    HeapI64 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_i64_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        int64_t tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_i64_sift_down(&h, 0);
    }
}
void double_heap_sort(double* arr, size_t n) 
{
    if (n < 2) return;
    HeapF64 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_f64_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        double tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_f64_sift_down(&h, 0);
    }
}

// Record Heap Sorts
void recu32_heap_sort(struct RecU32* arr, size_t n) 
{
    if (n < 2) return;
    HeapRecU32 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_recu32_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        struct RecU32 tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_recu32_sift_down(&h, 0);
    }
}
void reci32_heap_sort(struct RecI32* arr, size_t n) 
{
    if (n < 2) return;
    HeapRecI32 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_reci32_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        struct RecI32 tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_reci32_sift_down(&h, 0);
    }
}
void recf32_heap_sort(struct RecF32* arr, size_t n) 
{
    if (n < 2) return;
    HeapRecF32 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_recf32_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        struct RecF32 tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_recf32_sift_down(&h, 0);
    }
}
void recu64_heap_sort(struct RecU64* arr, size_t n) 
{
    if (n < 2) return;
    HeapRecU64 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_recu64_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        struct RecU64 tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_recu64_sift_down(&h, 0);
    }
}
void reci64_heap_sort(struct RecI64* arr, size_t n) 
{
    if (n < 2) return;
    HeapRecI64 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_reci64_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        struct RecI64 tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_reci64_sift_down(&h, 0);
    }
}
void recf64_heap_sort(struct RecF64* arr, size_t n) 
{
    if (n < 2) return;
    HeapRecF64 h;
    h.data = arr;
    h.size = n;
    h.capacity = n;
    h.is_min_heap = 0; 
    
    for (int i = (int)(n / 2) - 1; i >= 0; i--) 
    {
        heap_recf64_sift_down(&h, (size_t)i);
    }
    
    for (int i = (int)n - 1; i > 0; i--) 
    {
        struct RecF64 tmp = h.data[0];
        h.data[0] = h.data[i];
        h.data[i] = tmp;
        h.size--;
        heap_recf64_sift_down(&h, 0);
    }
}

void uint32_introsort(uint32_t* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            uint32_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid] < arr[low]) uint32_swap(&arr[mid], &arr[low]);
        if (arr[high] < arr[low]) uint32_swap(&arr[high], &arr[low]);
        if (arr[high] < arr[mid]) uint32_swap(&arr[high], &arr[mid]);
        uint32_t pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            if (i >= j) break;
            uint32_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            uint32_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            uint32_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    uint32_insertion(arr + low, high - low + 1);
}
void int32_introsort(int32_t* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            int32_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid] < arr[low]) int32_swap(&arr[mid], &arr[low]);
        if (arr[high] < arr[low]) int32_swap(&arr[high], &arr[low]);
        if (arr[high] < arr[mid]) int32_swap(&arr[high], &arr[mid]);
        int32_t pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            if (i >= j) break;
            int32_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            int32_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            int32_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    int32_insertion(arr + low, high - low + 1);
}
void float_introsort(float* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            float_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid] < arr[low]) float_swap(&arr[mid], &arr[low]);
        if (arr[high] < arr[low]) float_swap(&arr[high], &arr[low]);
        if (arr[high] < arr[mid]) float_swap(&arr[high], &arr[mid]);
        float pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            if (i >= j) break;
            float_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            float_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            float_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    float_insertion(arr + low, high - low + 1);
}
void uint64_introsort(uint64_t* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            uint64_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid] < arr[low]) uint64_swap(&arr[mid], &arr[low]);
        if (arr[high] < arr[low]) uint64_swap(&arr[high], &arr[low]);
        if (arr[high] < arr[mid]) uint64_swap(&arr[high], &arr[mid]);
        uint64_t pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            if (i >= j) break;
            uint64_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            uint64_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            uint64_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    uint64_insertion(arr + low, high - low + 1);
}
void int64_introsort(int64_t* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            int64_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid] < arr[low]) int64_swap(&arr[mid], &arr[low]);
        if (arr[high] < arr[low]) int64_swap(&arr[high], &arr[low]);
        if (arr[high] < arr[mid]) int64_swap(&arr[high], &arr[mid]);
        int64_t pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            if (i >= j) break;
            int64_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            int64_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            int64_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    int64_insertion(arr + low, high - low + 1);
}
void double_introsort(double* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            double_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid] < arr[low]) double_swap(&arr[mid], &arr[low]);
        if (arr[high] < arr[low]) double_swap(&arr[high], &arr[low]);
        if (arr[high] < arr[mid]) double_swap(&arr[high], &arr[mid]);
        double pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            if (i >= j) break;
            double_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            double_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            double_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    double_insertion(arr + low, high - low + 1);
}

// Record Introsorts
void recu32_introsort(struct RecU32* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            recu32_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid].key < arr[low].key) recu32_swap(&arr[mid], &arr[low]);
        if (arr[high].key < arr[low].key) recu32_swap(&arr[high], &arr[low]);
        if (arr[high].key < arr[mid].key) recu32_swap(&arr[high], &arr[mid]);
        struct RecU32 pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i].key < pivot.key);
            do { j--; } while (arr[j].key > pivot.key);
            if (i >= j) break;
            recu32_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            recu32_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            recu32_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    recu32_insertion(arr + low, high - low + 1);
}
void reci32_introsort(struct RecI32* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            reci32_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid].key < arr[low].key) reci32_swap(&arr[mid], &arr[low]);
        if (arr[high].key < arr[low].key) reci32_swap(&arr[high], &arr[low]);
        if (arr[high].key < arr[mid].key) reci32_swap(&arr[high], &arr[mid]);
        struct RecI32 pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i].key < pivot.key);
            do { j--; } while (arr[j].key > pivot.key);
            if (i >= j) break;
            reci32_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            reci32_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            reci32_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    reci32_insertion(arr + low, high - low + 1);
}
void recf32_introsort(struct RecF32* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            recf32_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid].key < arr[low].key) recf32_swap(&arr[mid], &arr[low]);
        if (arr[high].key < arr[low].key) recf32_swap(&arr[high], &arr[low]);
        if (arr[high].key < arr[mid].key) recf32_swap(&arr[high], &arr[mid]);
        struct RecF32 pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i].key < pivot.key);
            do { j--; } while (arr[j].key > pivot.key);
            if (i >= j) break;
            recf32_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            recf32_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            recf32_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    recf32_insertion(arr + low, high - low + 1);
}
void recu64_introsort(struct RecU64* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            recu64_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid].key < arr[low].key) recu64_swap(&arr[mid], &arr[low]);
        if (arr[high].key < arr[low].key) recu64_swap(&arr[high], &arr[low]);
        if (arr[high].key < arr[mid].key) recu64_swap(&arr[high], &arr[mid]);
        struct RecU64 pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i].key < pivot.key);
            do { j--; } while (arr[j].key > pivot.key);
            if (i >= j) break;
            recu64_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            recu64_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            recu64_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    recu64_insertion(arr + low, high - low + 1);
}
void reci64_introsort(struct RecI64* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            reci64_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid].key < arr[low].key) reci64_swap(&arr[mid], &arr[low]);
        if (arr[high].key < arr[low].key) reci64_swap(&arr[high], &arr[low]);
        if (arr[high].key < arr[mid].key) reci64_swap(&arr[high], &arr[mid]);
        struct RecI64 pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i].key < pivot.key);
            do { j--; } while (arr[j].key > pivot.key);
            if (i >= j) break;
            reci64_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            reci64_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            reci64_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    reci64_insertion(arr + low, high - low + 1);
}
void recf64_introsort(struct RecF64* arr, int low, int high, int depth_limit) 
{
    while (high - low > 16) 
    {
        if (depth_limit == 0) 
        { 
            recf64_heap_sort(arr + low, high - low + 1); 
            return; 
        }
        depth_limit--;
        int mid = low + (high - low) / 2;
        if (arr[mid].key < arr[low].key) recf64_swap(&arr[mid], &arr[low]);
        if (arr[high].key < arr[low].key) recf64_swap(&arr[high], &arr[low]);
        if (arr[high].key < arr[mid].key) recf64_swap(&arr[high], &arr[mid]);
        struct RecF64 pivot = arr[mid];
        int i = low - 1;
        int j = high + 1;
        while (1) 
        {
            do { i++; } while (arr[i].key < pivot.key);
            do { j--; } while (arr[j].key > pivot.key);
            if (i >= j) break;
            recf64_swap(&arr[i], &arr[j]);
        }
        if (j - low < high - j) 
        { 
            recf64_introsort(arr, low, j, depth_limit); 
            low = j + 1; 
        } 
        else 
        { 
            recf64_introsort(arr, j + 1, high, depth_limit); 
            high = j; 
        }
    }
    recf64_insertion(arr + low, high - low + 1);
}


void uint32_sort_fallback(uint32_t* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        uint32_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    uint32_introsort(arr, 0, (int)n - 1, depth_limit);
}
void int32_sort_fallback(int32_t* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        int32_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    int32_introsort(arr, 0, (int)n - 1, depth_limit);
}
void float_sort_fallback(float* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        float_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    float_introsort(arr, 0, (int)n - 1, depth_limit);
}
void uint64_sort_fallback(uint64_t* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        uint64_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    uint64_introsort(arr, 0, (int)n - 1, depth_limit);
}
void int64_sort_fallback(int64_t* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        int64_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    int64_introsort(arr, 0, (int)n - 1, depth_limit);
}
void double_sort_fallback(double* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        double_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    double_introsort(arr, 0, (int)n - 1, depth_limit);
}

// Record Fallbacks
void recu32_sort_fallback(struct RecU32* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        recu32_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    recu32_introsort(arr, 0, (int)n - 1, depth_limit);
}
void reci32_sort_fallback(struct RecI32* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        reci32_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    reci32_introsort(arr, 0, (int)n - 1, depth_limit);
}
void recf32_sort_fallback(struct RecF32* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        recf32_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    recf32_introsort(arr, 0, (int)n - 1, depth_limit);
}
void recu64_sort_fallback(struct RecU64* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        recu64_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    recu64_introsort(arr, 0, (int)n - 1, depth_limit);
}
void reci64_sort_fallback(struct RecI64* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        reci64_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    reci64_introsort(arr, 0, (int)n - 1, depth_limit);
}
void recf64_sort_fallback(struct RecF64* arr, size_t n) 
{
    if (n < 2) return;
    if (n <= 32) 
    {
        recf64_insertion(arr, n);
        return;
    }
    int depth_limit = 2 * fast_log2(n);
    recf64_introsort(arr, 0, (int)n - 1, depth_limit);
}

static inline uint32_t map_u32(uint32_t k) { return k; }
static inline uint64_t map_u64(uint64_t k) { return k; }
static inline uint32_t map_i32(int32_t k) { return (uint32_t)k ^ 0x80000000; }
static inline uint64_t map_i64(int64_t k) { return (uint64_t)k ^ 0x8000000000000000ULL; }

static inline uint32_t map_f32(float k) 
{ 
    uint32_t u; 
    memcpy(&u, &k, 4); 
    return u ^ ((-(u >> 31)) | 0x80000000); 
}

static inline uint64_t map_f64(double k) 
{ 
    uint64_t u; 
    memcpy(&u, &k, 8); 
    return u ^ ((-(u >> 63)) | 0x8000000000000000ULL); 
}


void universal_sort_uint32(uint32_t* arr, size_t n) 
{
    if (n < 1024) 
    { 
        uint32_sort_fallback(arr, n); 
        return; 
    }
    uint32_t* temp = malloc(n * sizeof(uint32_t));
    if (!temp) 
    { 
        uint32_sort_fallback(arr, n); 
        return; 
    }
    for (int p = 0; p < 4; p++) 
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_u32(arr[i]) >> shift) & 0xFF]++;
        }
        size_t pos[256]; 
        pos[0] = count[0];
        for (int i = 1; i < 256; i++) 
        {
            pos[i] = pos[i-1] + count[i];
        }
        for (size_t i = n; i > 0; i--) 
        {
            temp[--pos[(map_u32(arr[i-1]) >> shift) & 0xFF]] = arr[i-1];
        }
        memcpy(arr, temp, n * sizeof(uint32_t));
    }
    free(temp);
}
void universal_sort_int32(int32_t* arr, size_t n) 
{
    if (n < 1024) 
    { 
        int32_sort_fallback(arr, n); 
        return; 
    }
    int32_t* temp = malloc(n * sizeof(int32_t));
    if (!temp) 
    { 
        int32_sort_fallback(arr, n); 
        return; 
    }
    for (int p = 0; p < 4; p++) 
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_i32(arr[i]) >> shift) & 0xFF]++;
        }
        size_t pos[256]; 
        pos[0] = count[0];
        for (int i = 1; i < 256; i++) 
        {
            pos[i] = pos[i-1] + count[i];
        }
        for (size_t i = n; i > 0; i--) 
        {
            temp[--pos[(map_i32(arr[i-1]) >> shift) & 0xFF]] = arr[i-1];
        }
        memcpy(arr, temp, n * sizeof(int32_t));
    }
    free(temp);
}
void universal_sort_float(float* arr, size_t n) 
{
    if (n < 1024) 
    { 
        float_sort_fallback(arr, n); 
        return; 
    }
    float* temp = malloc(n * sizeof(float));
    if (!temp) 
    { 
        float_sort_fallback(arr, n); 
        return; 
    }
    for (int p = 0; p < 4; p++) 
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        for (size_t i = 0; i < n; i++)
        {
            count[(map_f32(arr[i]) >> shift) & 0xFF]++;
        }
        size_t pos[256]; 
        pos[0] = count[0];
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_f32(arr[i-1]) >> shift) & 0xFF]] = arr[i-1];
        }
        memcpy(arr, temp, n * sizeof(float));
    }
    free(temp);
}
void universal_sort_uint64(uint64_t* arr, size_t n) 
{
    if (n < 1024) 
    { 
        uint64_sort_fallback(arr, n); 
        return; 
    }
    uint64_t* temp = malloc(n * sizeof(uint64_t));
    if (!temp) 
    { 
        uint64_sort_fallback(arr, n); 
        return; 
    }
    for (int p = 0; p < 8; p++) 
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_u64(arr[i]) >> shift) & 0xFF]++;
        }
        size_t pos[256]; 
        pos[0] = count[0];
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_u64(arr[i-1]) >> shift) & 0xFF]] = arr[i-1];
        }
        memcpy(arr, temp, n * sizeof(uint64_t));
    }
    free(temp);
}
void universal_sort_int64(int64_t* arr, size_t n) 
{
    if (n < 1024) 
    { 
        int64_sort_fallback(arr, n); 
        return; 
    }
    int64_t* temp = malloc(n * sizeof(int64_t));
    if (!temp) 
    { 
        int64_sort_fallback(arr, n); 
        return; 
    }
    for (int p = 0; p < 8; p++) 
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        for (size_t i = 0; i < n; i++)
        {
            count[(map_i64(arr[i]) >> shift) & 0xFF]++;
        }
        size_t pos[256]; 
        pos[0] = count[0];
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_i64(arr[i-1]) >> shift) & 0xFF]] = arr[i-1];
        }
        memcpy(arr, temp, n * sizeof(int64_t));
    }
    free(temp);
}
void universal_sort_double(double* arr, size_t n) 
{
    if (n < 1024) 
    { 
        double_sort_fallback(arr, n); 
        return; 
    }
    double* temp = malloc(n * sizeof(double));
    if (!temp) 
    { 
        double_sort_fallback(arr, n); 
        return; 
    }
    for (int p = 0; p < 8; p++) 
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_f64(arr[i]) >> shift) & 0xFF]++;
        }
        size_t pos[256]; 
        pos[0] = count[0];
        for (int i = 1; i < 256; i++) 
        {
            pos[i] = pos[i-1] + count[i];
        }
        for (size_t i = n; i > 0; i--) 
        {
            temp[--pos[(map_f64(arr[i-1]) >> shift) & 0xFF]] = arr[i-1];
        }
        memcpy(arr, temp, n * sizeof(double));
    }
    free(temp);
}

// Record Universal Sorts
void universal_sort_recu32(struct RecU32* arr, size_t n) 
{
    if (n < 1024) 
    {
        recu32_sort_fallback(arr, n);
        return;
    }
    
    struct RecU32* temp = malloc(n * sizeof(struct RecU32));
    if (!temp) 
    { 
        recu32_sort_fallback(arr, n);
        return;
    }
    
    for (int p = 0; p < 4; p++)
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_u32(arr[i].key) >> shift) & 0xFF]++;
        }
        
        size_t pos[256]; 
        pos[0] = count[0];
        
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_u32(arr[i-1].key) >> shift) & 0xFF]] = arr[i-1];
        }
        
        memcpy(arr, temp, n * sizeof(struct RecU32));
    }
    free(temp);
}
void universal_sort_reci32(struct RecI32* arr, size_t n) 
{
    if (n < 1024) 
    {
        reci32_sort_fallback(arr, n);
        return;
    }
    
    struct RecI32* temp = malloc(n * sizeof(struct RecI32));
    if (!temp) 
    { 
        reci32_sort_fallback(arr, n);
        return;
    }
    
    for (int p = 0; p < 4; p++)
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_i32(arr[i].key) >> shift) & 0xFF]++;
        }
        
        size_t pos[256]; 
        pos[0] = count[0];
        
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_i32(arr[i-1].key) >> shift) & 0xFF]] = arr[i-1];
        }
        
        memcpy(arr, temp, n * sizeof(struct RecI32));
    }
    free(temp);
}
void universal_sort_recf32(struct RecF32* arr, size_t n) 
{
    if (n < 1024) 
    {
        recf32_sort_fallback(arr, n);
        return;
    }
    
    struct RecF32* temp = malloc(n * sizeof(struct RecF32));
    if (!temp) 
    { 
        recf32_sort_fallback(arr, n);
        return;
    }
    
    for (int p = 0; p < 4; p++)
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_f32(arr[i].key) >> shift) & 0xFF]++;
        }
        
        size_t pos[256]; 
        pos[0] = count[0];
        
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_f32(arr[i-1].key) >> shift) & 0xFF]] = arr[i-1];
        }
        
        memcpy(arr, temp, n * sizeof(struct RecF32));
    }
    free(temp);
}
void universal_sort_recu64(struct RecU64* arr, size_t n) 
{
    if (n < 1024) 
    {
        recu64_sort_fallback(arr, n);
        return;
    }
    
    struct RecU64* temp = malloc(n * sizeof(struct RecU64));
    if (!temp) 
    { 
        recu64_sort_fallback(arr, n);
        return;
    }
    
    for (int p = 0; p < 8; p++)
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_u64(arr[i].key) >> shift) & 0xFF]++;
        }
        
        size_t pos[256]; 
        pos[0] = count[0];
        
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_u64(arr[i-1].key) >> shift) & 0xFF]] = arr[i-1];
        }
        
        memcpy(arr, temp, n * sizeof(struct RecU64));
    }
    free(temp);
}
void universal_sort_reci64(struct RecI64* arr, size_t n) 
{
    if (n < 1024) 
    {
        reci64_sort_fallback(arr, n);
        return;
    }
    
    struct RecI64* temp = malloc(n * sizeof(struct RecI64));
    if (!temp) 
    { 
        reci64_sort_fallback(arr, n);
        return;
    }
    
    for (int p = 0; p < 8; p++)
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_i64(arr[i].key) >> shift) & 0xFF]++;
        }
        
        size_t pos[256]; 
        pos[0] = count[0];
        
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_i64(arr[i-1].key) >> shift) & 0xFF]] = arr[i-1];
        }
        
        memcpy(arr, temp, n * sizeof(struct RecI64));
    }
    free(temp);
}
void universal_sort_recf64(struct RecF64* arr, size_t n) 
{
    if (n < 1024) 
    {
        recf64_sort_fallback(arr, n);
        return;
    }
    
    struct RecF64* temp = malloc(n * sizeof(struct RecF64));
    if (!temp) 
    { 
        recf64_sort_fallback(arr, n);
        return;
    }
    
    for (int p = 0; p < 8; p++)
    {
        int shift = p * 8; 
        size_t count[256] = {0};
        
        for (size_t i = 0; i < n; i++) 
        {
            count[(map_f64(arr[i].key) >> shift) & 0xFF]++;
        }
        
        size_t pos[256]; 
        pos[0] = count[0];
        
        for (int i = 1; i < 256; i++)
        {
            pos[i] = pos[i-1] + count[i];
        }
        
        for (size_t i = n; i > 0; i--)
        {
            temp[--pos[(map_f64(arr[i-1].key) >> shift) & 0xFF]] = arr[i-1];
        }
        
        memcpy(arr, temp, n * sizeof(struct RecF64));
    }
    free(temp);
}