#include "heap.h"
#include <string.h>


static void heap_u32_sift_up(HeapU32* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;
        if (heap->is_min_heap) swap_needed = (heap->data[idx] < heap->data[parent]);
        else swap_needed = (heap->data[idx] > heap->data[parent]);

        if (swap_needed) 
        {
            uint32_t tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_u32_sift_down(HeapU32* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left] < heap->data[target]) target = left;
            if (right < size && heap->data[right] < heap->data[target]) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left] > heap->data[target]) target = left;
            if (right < size && heap->data[right] > heap->data[target]) target = right;
        }

        if (target != idx) 
        {
            uint32_t tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapU32* heap_u32_init(size_t initial_capacity, int is_min_heap) 
{
    HeapU32* heap = (HeapU32*)malloc(sizeof(HeapU32));
    if (!heap) return NULL;
    if (initial_capacity == 0) initial_capacity = 16;
    heap->data = (uint32_t*)malloc(initial_capacity * sizeof(uint32_t));
    if (!heap->data) { free(heap); return NULL; }
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapU32* heap_u32_heapify(const uint32_t* arr, size_t n, int is_min_heap)
{
    HeapU32* heap = heap_u32_init(n, is_min_heap);
    if (!heap) return NULL;
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(uint32_t));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) heap_u32_sift_down(heap, (size_t)i);
    }
    return heap;
}

void heap_u32_free(HeapU32* heap) 
{
    if (heap) { if (heap->data) free(heap->data); free(heap); }
}

int heap_u32_insert(HeapU32* heap, uint32_t item) 
{
    if (!heap) return 0;
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        uint32_t* new_data = (uint32_t*)realloc(heap->data, new_cap * sizeof(uint32_t));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    heap->data[heap->size] = item;
    heap_u32_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_u32_extract(HeapU32* heap, uint32_t* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0) heap_u32_sift_down(heap, 0);
    return 1;
}

int heap_u32_top(const HeapU32* heap, uint32_t* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// I32
static void heap_i32_sift_up(HeapI32* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;
        if (heap->is_min_heap) swap_needed = (heap->data[idx] < heap->data[parent]);
        else swap_needed = (heap->data[idx] > heap->data[parent]);

        if (swap_needed) 
        {
            int32_t tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_i32_sift_down(HeapI32* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left] < heap->data[target]) target = left;
            if (right < size && heap->data[right] < heap->data[target]) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left] > heap->data[target]) target = left;
            if (right < size && heap->data[right] > heap->data[target]) target = right;
        }

        if (target != idx) 
        {
            int32_t tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapI32* heap_i32_init(size_t initial_capacity, int is_min_heap) 
{
    HeapI32* heap = (HeapI32*)malloc(sizeof(HeapI32));
    if (!heap) return NULL;
    if (initial_capacity == 0) initial_capacity = 16;
    heap->data = (int32_t*)malloc(initial_capacity * sizeof(int32_t));
    if (!heap->data) { free(heap); return NULL; }
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapI32* heap_i32_heapify(const int32_t* arr, size_t n, int is_min_heap)
{
    HeapI32* heap = heap_i32_init(n, is_min_heap);
    if (!heap) return NULL;
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(int32_t));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) heap_i32_sift_down(heap, (size_t)i);
    }
    return heap;
}

void heap_i32_free(HeapI32* heap) 
{
    if (heap) { if (heap->data) free(heap->data); free(heap); }
}

int heap_i32_insert(HeapI32* heap, int32_t item) 
{
    if (!heap) return 0;
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        int32_t* new_data = (int32_t*)realloc(heap->data, new_cap * sizeof(int32_t));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    heap->data[heap->size] = item;
    heap_i32_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_i32_extract(HeapI32* heap, int32_t* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0) heap_i32_sift_down(heap, 0);
    return 1;
}

int heap_i32_top(const HeapI32* heap, int32_t* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// F32
static void heap_f32_sift_up(HeapF32* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;
        if (heap->is_min_heap) swap_needed = (heap->data[idx] < heap->data[parent]);
        else swap_needed = (heap->data[idx] > heap->data[parent]);

        if (swap_needed) 
        {
            float tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_f32_sift_down(HeapF32* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left] < heap->data[target]) target = left;
            if (right < size && heap->data[right] < heap->data[target]) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left] > heap->data[target]) target = left;
            if (right < size && heap->data[right] > heap->data[target]) target = right;
        }

        if (target != idx) 
        {
            float tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapF32* heap_f32_init(size_t initial_capacity, int is_min_heap) 
{
    HeapF32* heap = (HeapF32*)malloc(sizeof(HeapF32));
    if (!heap) return NULL;
    if (initial_capacity == 0) initial_capacity = 16;
    heap->data = (float*)malloc(initial_capacity * sizeof(float));
    if (!heap->data) { free(heap); return NULL; }
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapF32* heap_f32_heapify(const float* arr, size_t n, int is_min_heap)
{
    HeapF32* heap = heap_f32_init(n, is_min_heap);
    if (!heap) return NULL;
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(float));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) heap_f32_sift_down(heap, (size_t)i);
    }
    return heap;
}

void heap_f32_free(HeapF32* heap) 
{
    if (heap) { if (heap->data) free(heap->data); free(heap); }
}

int heap_f32_insert(HeapF32* heap, float item) 
{
    if (!heap) return 0;
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        float* new_data = (float*)realloc(heap->data, new_cap * sizeof(float));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    heap->data[heap->size] = item;
    heap_f32_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_f32_extract(HeapF32* heap, float* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0) heap_f32_sift_down(heap, 0);
    return 1;
}

int heap_f32_top(const HeapF32* heap, float* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// U64
static void heap_u64_sift_up(HeapU64* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;
        if (heap->is_min_heap) swap_needed = (heap->data[idx] < heap->data[parent]);
        else swap_needed = (heap->data[idx] > heap->data[parent]);

        if (swap_needed) 
        {
            uint64_t tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_u64_sift_down(HeapU64* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left] < heap->data[target]) target = left;
            if (right < size && heap->data[right] < heap->data[target]) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left] > heap->data[target]) target = left;
            if (right < size && heap->data[right] > heap->data[target]) target = right;
        }

        if (target != idx) 
        {
            uint64_t tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapU64* heap_u64_init(size_t initial_capacity, int is_min_heap) 
{
    HeapU64* heap = (HeapU64*)malloc(sizeof(HeapU64));
    if (!heap) return NULL;
    if (initial_capacity == 0) initial_capacity = 16;
    heap->data = (uint64_t*)malloc(initial_capacity * sizeof(uint64_t));
    if (!heap->data) { free(heap); return NULL; }
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapU64* heap_u64_heapify(const uint64_t* arr, size_t n, int is_min_heap)
{
    HeapU64* heap = heap_u64_init(n, is_min_heap);
    if (!heap) return NULL;
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(uint64_t));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) heap_u64_sift_down(heap, (size_t)i);
    }
    return heap;
}

void heap_u64_free(HeapU64* heap) 
{
    if (heap) { if (heap->data) free(heap->data); free(heap); }
}

int heap_u64_insert(HeapU64* heap, uint64_t item) 
{
    if (!heap) return 0;
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        uint64_t* new_data = (uint64_t*)realloc(heap->data, new_cap * sizeof(uint64_t));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    heap->data[heap->size] = item;
    heap_u64_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_u64_extract(HeapU64* heap, uint64_t* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0) heap_u64_sift_down(heap, 0);
    return 1;
}

int heap_u64_top(const HeapU64* heap, uint64_t* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// I64
static void heap_i64_sift_up(HeapI64* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;
        if (heap->is_min_heap) swap_needed = (heap->data[idx] < heap->data[parent]);
        else swap_needed = (heap->data[idx] > heap->data[parent]);

        if (swap_needed) 
        {
            int64_t tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_i64_sift_down(HeapI64* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left] < heap->data[target]) target = left;
            if (right < size && heap->data[right] < heap->data[target]) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left] > heap->data[target]) target = left;
            if (right < size && heap->data[right] > heap->data[target]) target = right;
        }

        if (target != idx) 
        {
            int64_t tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapI64* heap_i64_init(size_t initial_capacity, int is_min_heap) 
{
    HeapI64* heap = (HeapI64*)malloc(sizeof(HeapI64));
    if (!heap) return NULL;
    if (initial_capacity == 0) initial_capacity = 16;
    heap->data = (int64_t*)malloc(initial_capacity * sizeof(int64_t));
    if (!heap->data) { free(heap); return NULL; }
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapI64* heap_i64_heapify(const int64_t* arr, size_t n, int is_min_heap)
{
    HeapI64* heap = heap_i64_init(n, is_min_heap);
    if (!heap) return NULL;
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(int64_t));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) heap_i64_sift_down(heap, (size_t)i);
    }
    return heap;
}

void heap_i64_free(HeapI64* heap) 
{
    if (heap) { if (heap->data) free(heap->data); free(heap); }
}

int heap_i64_insert(HeapI64* heap, int64_t item) 
{
    if (!heap) return 0;
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        int64_t* new_data = (int64_t*)realloc(heap->data, new_cap * sizeof(int64_t));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    heap->data[heap->size] = item;
    heap_i64_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_i64_extract(HeapI64* heap, int64_t* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0) heap_i64_sift_down(heap, 0);
    return 1;
}

int heap_i64_top(const HeapI64* heap, int64_t* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// F64
static void heap_f64_sift_up(HeapF64* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;
        if (heap->is_min_heap) swap_needed = (heap->data[idx] < heap->data[parent]);
        else swap_needed = (heap->data[idx] > heap->data[parent]);

        if (swap_needed) 
        {
            double tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_f64_sift_down(HeapF64* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left] < heap->data[target]) target = left;
            if (right < size && heap->data[right] < heap->data[target]) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left] > heap->data[target]) target = left;
            if (right < size && heap->data[right] > heap->data[target]) target = right;
        }

        if (target != idx) 
        {
            double tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapF64* heap_f64_init(size_t initial_capacity, int is_min_heap) 
{
    HeapF64* heap = (HeapF64*)malloc(sizeof(HeapF64));
    if (!heap) return NULL;
    if (initial_capacity == 0) initial_capacity = 16;
    heap->data = (double*)malloc(initial_capacity * sizeof(double));
    if (!heap->data) { free(heap); return NULL; }
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapF64* heap_f64_heapify(const double* arr, size_t n, int is_min_heap)
{
    HeapF64* heap = heap_f64_init(n, is_min_heap);
    if (!heap) return NULL;
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(double));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) heap_f64_sift_down(heap, (size_t)i);
    }
    return heap;
}

void heap_f64_free(HeapF64* heap) 
{
    if (heap) { if (heap->data) free(heap->data); free(heap); }
}

int heap_f64_insert(HeapF64* heap, double item) 
{
    if (!heap) return 0;
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        double* new_data = (double*)realloc(heap->data, new_cap * sizeof(double));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    heap->data[heap->size] = item;
    heap_f64_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_f64_extract(HeapF64* heap, double* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0) heap_f64_sift_down(heap, 0);
    return 1;
}

int heap_f64_top(const HeapF64* heap, double* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}


static void heap_recu32_sift_up(HeapRecU32* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;

        if (heap->is_min_heap) 
        {
            swap_needed = (heap->data[idx].key < heap->data[parent].key);
        } 
        else 
        {
            swap_needed = (heap->data[idx].key > heap->data[parent].key);
        }

        if (swap_needed) 
        {
            struct RecU32 tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_recu32_sift_down(HeapRecU32* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left].key < heap->data[target].key) target = left;
            if (right < size && heap->data[right].key < heap->data[target].key) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left].key > heap->data[target].key) target = left;
            if (right < size && heap->data[right].key > heap->data[target].key) target = right;
        }

        if (target != idx) 
        {
            struct RecU32 tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapRecU32* heap_recu32_init(size_t initial_capacity, int is_min_heap) 
{
    HeapRecU32* heap = (HeapRecU32*)malloc(sizeof(HeapRecU32));
    if (!heap) return NULL;
    
    if (initial_capacity == 0) initial_capacity = 16;
    
    heap->data = (struct RecU32*)malloc(initial_capacity * sizeof(struct RecU32));
    if (!heap->data) 
    {
        free(heap);
        return NULL;
    }
    
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapRecU32* heap_recu32_heapify(const struct RecU32* arr, size_t n, int is_min_heap)
{
    HeapRecU32* heap = heap_recu32_init(n, is_min_heap);
    if (!heap) return NULL;
    
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(struct RecU32));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) 
        {
            heap_recu32_sift_down(heap, (size_t)i);
        }
    }
    return heap;
}

void heap_recu32_free(HeapRecU32* heap) 
{
    if (heap) 
    {
        if (heap->data) free(heap->data);
        free(heap);
    }
}

int heap_recu32_insert(HeapRecU32* heap, struct RecU32 item) 
{
    if (!heap) return 0;
    
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        struct RecU32* new_data = (struct RecU32*)realloc(heap->data, new_cap * sizeof(struct RecU32));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    
    heap->data[heap->size] = item;
    heap_recu32_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_recu32_extract(HeapRecU32* heap, struct RecU32* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    
    if (out_item) *out_item = heap->data[0];
    
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    
    if (heap->size > 0) 
    {
        heap_recu32_sift_down(heap, 0);
    }
    return 1;
}

int heap_recu32_top(const HeapRecU32* heap, struct RecU32* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// RECI32
static void heap_reci32_sift_up(HeapRecI32* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;

        if (heap->is_min_heap) 
        {
            swap_needed = (heap->data[idx].key < heap->data[parent].key);
        } 
        else 
        {
            swap_needed = (heap->data[idx].key > heap->data[parent].key);
        }

        if (swap_needed) 
        {
            struct RecI32 tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_reci32_sift_down(HeapRecI32* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left].key < heap->data[target].key) target = left;
            if (right < size && heap->data[right].key < heap->data[target].key) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left].key > heap->data[target].key) target = left;
            if (right < size && heap->data[right].key > heap->data[target].key) target = right;
        }

        if (target != idx) 
        {
            struct RecI32 tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapRecI32* heap_reci32_init(size_t initial_capacity, int is_min_heap) 
{
    HeapRecI32* heap = (HeapRecI32*)malloc(sizeof(HeapRecI32));
    if (!heap) return NULL;
    
    if (initial_capacity == 0) initial_capacity = 16;
    
    heap->data = (struct RecI32*)malloc(initial_capacity * sizeof(struct RecI32));
    if (!heap->data) 
    {
        free(heap);
        return NULL;
    }
    
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapRecI32* heap_reci32_heapify(const struct RecI32* arr, size_t n, int is_min_heap)
{
    HeapRecI32* heap = heap_reci32_init(n, is_min_heap);
    if (!heap) return NULL;
    
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(struct RecI32));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) 
        {
            heap_reci32_sift_down(heap, (size_t)i);
        }
    }
    return heap;
}

void heap_reci32_free(HeapRecI32* heap) 
{
    if (heap) 
    {
        if (heap->data) free(heap->data);
        free(heap);
    }
}

int heap_reci32_insert(HeapRecI32* heap, struct RecI32 item) 
{
    if (!heap) return 0;
    
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        struct RecI32* new_data = (struct RecI32*)realloc(heap->data, new_cap * sizeof(struct RecI32));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    
    heap->data[heap->size] = item;
    heap_reci32_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_reci32_extract(HeapRecI32* heap, struct RecI32* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    
    if (out_item) *out_item = heap->data[0];
    
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    
    if (heap->size > 0) 
    {
        heap_reci32_sift_down(heap, 0);
    }
    return 1;
}

int heap_reci32_top(const HeapRecI32* heap, struct RecI32* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// RECF32
static void heap_recf32_sift_up(HeapRecF32* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;

        if (heap->is_min_heap) 
        {
            swap_needed = (heap->data[idx].key < heap->data[parent].key);
        } 
        else 
        {
            swap_needed = (heap->data[idx].key > heap->data[parent].key);
        }

        if (swap_needed) 
        {
            struct RecF32 tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_recf32_sift_down(HeapRecF32* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left].key < heap->data[target].key) target = left;
            if (right < size && heap->data[right].key < heap->data[target].key) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left].key > heap->data[target].key) target = left;
            if (right < size && heap->data[right].key > heap->data[target].key) target = right;
        }

        if (target != idx) 
        {
            struct RecF32 tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapRecF32* heap_recf32_init(size_t initial_capacity, int is_min_heap) 
{
    HeapRecF32* heap = (HeapRecF32*)malloc(sizeof(HeapRecF32));
    if (!heap) return NULL;
    
    if (initial_capacity == 0) initial_capacity = 16;
    
    heap->data = (struct RecF32*)malloc(initial_capacity * sizeof(struct RecF32));
    if (!heap->data) 
    {
        free(heap);
        return NULL;
    }
    
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapRecF32* heap_recf32_heapify(const struct RecF32* arr, size_t n, int is_min_heap)
{
    HeapRecF32* heap = heap_recf32_init(n, is_min_heap);
    if (!heap) return NULL;
    
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(struct RecF32));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) 
        {
            heap_recf32_sift_down(heap, (size_t)i);
        }
    }
    return heap;
}

void heap_recf32_free(HeapRecF32* heap) 
{
    if (heap) 
    {
        if (heap->data) free(heap->data);
        free(heap);
    }
}

int heap_recf32_insert(HeapRecF32* heap, struct RecF32 item) 
{
    if (!heap) return 0;
    
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        struct RecF32* new_data = (struct RecF32*)realloc(heap->data, new_cap * sizeof(struct RecF32));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    
    heap->data[heap->size] = item;
    heap_recf32_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_recf32_extract(HeapRecF32* heap, struct RecF32* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    
    if (out_item) *out_item = heap->data[0];
    
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    
    if (heap->size > 0) 
    {
        heap_recf32_sift_down(heap, 0);
    }
    return 1;
}

int heap_recf32_top(const HeapRecF32* heap, struct RecF32* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// RECU64
static void heap_recu64_sift_up(HeapRecU64* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;

        if (heap->is_min_heap) 
        {
            swap_needed = (heap->data[idx].key < heap->data[parent].key);
        } 
        else 
        {
            swap_needed = (heap->data[idx].key > heap->data[parent].key);
        }

        if (swap_needed) 
        {
            struct RecU64 tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_recu64_sift_down(HeapRecU64* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left].key < heap->data[target].key) target = left;
            if (right < size && heap->data[right].key < heap->data[target].key) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left].key > heap->data[target].key) target = left;
            if (right < size && heap->data[right].key > heap->data[target].key) target = right;
        }

        if (target != idx) 
        {
            struct RecU64 tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapRecU64* heap_recu64_init(size_t initial_capacity, int is_min_heap) 
{
    HeapRecU64* heap = (HeapRecU64*)malloc(sizeof(HeapRecU64));
    if (!heap) return NULL;
    
    if (initial_capacity == 0) initial_capacity = 16;
    
    heap->data = (struct RecU64*)malloc(initial_capacity * sizeof(struct RecU64));
    if (!heap->data) 
    {
        free(heap);
        return NULL;
    }
    
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapRecU64* heap_recu64_heapify(const struct RecU64* arr, size_t n, int is_min_heap)
{
    HeapRecU64* heap = heap_recu64_init(n, is_min_heap);
    if (!heap) return NULL;
    
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(struct RecU64));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) 
        {
            heap_recu64_sift_down(heap, (size_t)i);
        }
    }
    return heap;
}

void heap_recu64_free(HeapRecU64* heap) 
{
    if (heap) 
    {
        if (heap->data) free(heap->data);
        free(heap);
    }
}

int heap_recu64_insert(HeapRecU64* heap, struct RecU64 item) 
{
    if (!heap) return 0;
    
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        struct RecU64* new_data = (struct RecU64*)realloc(heap->data, new_cap * sizeof(struct RecU64));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    
    heap->data[heap->size] = item;
    heap_recu64_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_recu64_extract(HeapRecU64* heap, struct RecU64* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    
    if (out_item) *out_item = heap->data[0];
    
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    
    if (heap->size > 0) 
    {
        heap_recu64_sift_down(heap, 0);
    }
    return 1;
}

int heap_recu64_top(const HeapRecU64* heap, struct RecU64* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// RECI64
static void heap_reci64_sift_up(HeapRecI64* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;

        if (heap->is_min_heap) 
        {
            swap_needed = (heap->data[idx].key < heap->data[parent].key);
        } 
        else 
        {
            swap_needed = (heap->data[idx].key > heap->data[parent].key);
        }

        if (swap_needed) 
        {
            struct RecI64 tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_reci64_sift_down(HeapRecI64* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left].key < heap->data[target].key) target = left;
            if (right < size && heap->data[right].key < heap->data[target].key) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left].key > heap->data[target].key) target = left;
            if (right < size && heap->data[right].key > heap->data[target].key) target = right;
        }

        if (target != idx) 
        {
            struct RecI64 tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapRecI64* heap_reci64_init(size_t initial_capacity, int is_min_heap) 
{
    HeapRecI64* heap = (HeapRecI64*)malloc(sizeof(HeapRecI64));
    if (!heap) return NULL;
    
    if (initial_capacity == 0) initial_capacity = 16;
    
    heap->data = (struct RecI64*)malloc(initial_capacity * sizeof(struct RecI64));
    if (!heap->data) 
    {
        free(heap);
        return NULL;
    }
    
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapRecI64* heap_reci64_heapify(const struct RecI64* arr, size_t n, int is_min_heap)
{
    HeapRecI64* heap = heap_reci64_init(n, is_min_heap);
    if (!heap) return NULL;
    
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(struct RecI64));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) 
        {
            heap_reci64_sift_down(heap, (size_t)i);
        }
    }
    return heap;
}

void heap_reci64_free(HeapRecI64* heap) 
{
    if (heap) 
    {
        if (heap->data) free(heap->data);
        free(heap);
    }
}

int heap_reci64_insert(HeapRecI64* heap, struct RecI64 item) 
{
    if (!heap) return 0;
    
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        struct RecI64* new_data = (struct RecI64*)realloc(heap->data, new_cap * sizeof(struct RecI64));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    
    heap->data[heap->size] = item;
    heap_reci64_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_reci64_extract(HeapRecI64* heap, struct RecI64* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    
    if (out_item) *out_item = heap->data[0];
    
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    
    if (heap->size > 0) 
    {
        heap_reci64_sift_down(heap, 0);
    }
    return 1;
}

int heap_reci64_top(const HeapRecI64* heap, struct RecI64* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}

// RECF64
static void heap_recf64_sift_up(HeapRecF64* heap, size_t idx) 
{
    while (idx > 0) 
    {
        size_t parent = (idx - 1) / 2;
        int swap_needed = 0;

        if (heap->is_min_heap) 
        {
            swap_needed = (heap->data[idx].key < heap->data[parent].key);
        } 
        else 
        {
            swap_needed = (heap->data[idx].key > heap->data[parent].key);
        }

        if (swap_needed) 
        {
            struct RecF64 tmp = heap->data[idx];
            heap->data[idx] = heap->data[parent];
            heap->data[parent] = tmp;
            idx = parent;
        } 
        else break;
    }
}

void heap_recf64_sift_down(HeapRecF64* heap, size_t idx) 
{
    size_t size = heap->size;
    while (1) 
    {
        size_t left = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t target = idx;

        if (heap->is_min_heap) 
        {
            if (left < size && heap->data[left].key < heap->data[target].key) target = left;
            if (right < size && heap->data[right].key < heap->data[target].key) target = right;
        } 
        else 
        {
            if (left < size && heap->data[left].key > heap->data[target].key) target = left;
            if (right < size && heap->data[right].key > heap->data[target].key) target = right;
        }

        if (target != idx) 
        {
            struct RecF64 tmp = heap->data[idx];
            heap->data[idx] = heap->data[target];
            heap->data[target] = tmp;
            idx = target;
        } 
        else break;
    }
}

HeapRecF64* heap_recf64_init(size_t initial_capacity, int is_min_heap) 
{
    HeapRecF64* heap = (HeapRecF64*)malloc(sizeof(HeapRecF64));
    if (!heap) return NULL;
    
    if (initial_capacity == 0) initial_capacity = 16;
    
    heap->data = (struct RecF64*)malloc(initial_capacity * sizeof(struct RecF64));
    if (!heap->data) 
    {
        free(heap);
        return NULL;
    }
    
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

HeapRecF64* heap_recf64_heapify(const struct RecF64* arr, size_t n, int is_min_heap)
{
    HeapRecF64* heap = heap_recf64_init(n, is_min_heap);
    if (!heap) return NULL;
    
    if (n > 0 && arr) 
    {
        memcpy(heap->data, arr, n * sizeof(struct RecF64));
        heap->size = n;
        for (int i = (int)(n / 2) - 1; i >= 0; i--) 
        {
            heap_recf64_sift_down(heap, (size_t)i);
        }
    }
    return heap;
}

void heap_recf64_free(HeapRecF64* heap) 
{
    if (heap) 
    {
        if (heap->data) free(heap->data);
        free(heap);
    }
}

int heap_recf64_insert(HeapRecF64* heap, struct RecF64 item) 
{
    if (!heap) return 0;
    
    if (heap->size >= heap->capacity) 
    {
        size_t new_cap = heap->capacity * 2;
        struct RecF64* new_data = (struct RecF64*)realloc(heap->data, new_cap * sizeof(struct RecF64));
        if (!new_data) return 0;
        heap->data = new_data;
        heap->capacity = new_cap;
    }
    
    heap->data[heap->size] = item;
    heap_recf64_sift_up(heap, heap->size);
    heap->size++;
    return 1;
}

int heap_recf64_extract(HeapRecF64* heap, struct RecF64* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    
    if (out_item) *out_item = heap->data[0];
    
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    
    if (heap->size > 0) 
    {
        heap_recf64_sift_down(heap, 0);
    }
    return 1;
}

int heap_recf64_top(const HeapRecF64* heap, struct RecF64* out_item) 
{
    if (!heap || heap->size == 0) return 0;
    if (out_item) *out_item = heap->data[0];
    return 1;
}