#include "dynamic_table.h"
// #include "macros.h"
#include <stdio.h>
#include "sort.h"


void init_RecI32_dt(struct RecI32_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct RecI32_dt));
    if (*table_ptr == NULL)
    {
        return; 
    }
    (*table_ptr)->table = (struct RecI32 *) malloc(((size_t) BASE_DT_SIZE) * sizeof(struct RecI32));
    if ((*table_ptr)->table == NULL) 
    {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    } 
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_RecI32_dt(struct RecI32_dt *table, struct RecI32 *rec)
{
    if(table->element_cnt == table->arr_size)
    {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecI32));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt].data = rec->data;
    table->table[table->element_cnt].key = rec->key;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_RecI32_dt(struct RecI32_dt *table, struct RecI32 *deleted_data)
{
    if(table->element_cnt == 0)
    {
        return;
    }
    if(deleted_data != NULL)
    {
        deleted_data->data = table->table[table->element_cnt - 1].data;
        deleted_data->key = table->table[table->element_cnt - 1].key;
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE)
    {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecI32));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}

int linear_search_RecI32_dt(struct RecI32_dt *table, int32_t key, size_t *position)
{
    struct RecI32 *begin = table->table;
    struct RecI32 *end = begin + table->element_cnt;
#pragma GCC unroll 4
    for(struct RecI32 *ptr = begin; ptr < end; ptr++)
    {
        if( ptr->key == key)
        {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}


int binary_search_RecI32_dt(struct RecI32_dt *table, int32_t key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_reci32(table->table, table->element_cnt);
        table->is_sorted = 1;
    }

    struct RecI32 *low = table->table;
    struct RecI32 *high = low + table->element_cnt;
    struct RecI32 *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (mid->key < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && low->key == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }

    return 0;
 
}


void init_RecI64_dt(struct RecI64_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct RecI64_dt));
    if (*table_ptr == NULL) return; 

    (*table_ptr)->table = (struct RecI64 *) malloc(((size_t) BASE_DT_SIZE) * sizeof(struct RecI64));
    if ((*table_ptr)->table == NULL) 
    {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    } 
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_RecI64_dt(struct RecI64_dt *table, struct RecI64 *rec)
{
    if(table->element_cnt == table->arr_size)
    {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecI64));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt].data = rec->data;
    table->table[table->element_cnt].key = rec->key;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_RecI64_dt(struct RecI64_dt *table, struct RecI64 *deleted_data)
{
    if(table->element_cnt == 0) return;

    if(deleted_data != NULL)
    {
        deleted_data->data = table->table[table->element_cnt - 1].data;
        deleted_data->key = table->table[table->element_cnt - 1].key;
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE)
    {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecI64));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}


void init_RecU32_dt(struct RecU32_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct RecU32_dt));
    if (*table_ptr == NULL) return; 

    (*table_ptr)->table = (struct RecU32 *) malloc(((size_t) BASE_DT_SIZE) * sizeof(struct RecU32));
    if ((*table_ptr)->table == NULL) 
    {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    } 
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_RecU32_dt(struct RecU32_dt *table, struct RecU32 *rec)
{
    if(table->element_cnt == table->arr_size)
    {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecU32));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt].data = rec->data;
    table->table[table->element_cnt].key = rec->key;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_RecU32_dt(struct RecU32_dt *table, struct RecU32 *deleted_data)
{
    if(table->element_cnt == 0) return;

    if(deleted_data != NULL)
    {
        deleted_data->data = table->table[table->element_cnt - 1].data;
        deleted_data->key = table->table[table->element_cnt - 1].key;
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE)
    {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecU32));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}



void init_RecU64_dt(struct RecU64_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct RecU64_dt));
    if (*table_ptr == NULL) return; 

    (*table_ptr)->table = (struct RecU64 *) malloc(((size_t) BASE_DT_SIZE) * sizeof(struct RecU64));
    if ((*table_ptr)->table == NULL) 
    {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    } 
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_RecU64_dt(struct RecU64_dt *table, struct RecU64 *rec)
{
    if(table->element_cnt == table->arr_size)
    {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecU64));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt].data = rec->data;
    table->table[table->element_cnt].key = rec->key;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_RecU64_dt(struct RecU64_dt *table, struct RecU64 *deleted_data)
{
    if(table->element_cnt == 0) return;

    if(deleted_data != NULL)
    {
        deleted_data->data = table->table[table->element_cnt - 1].data;
        deleted_data->key = table->table[table->element_cnt - 1].key;
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE)
    {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecU64));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}




void init_RecF32_dt(struct RecF32_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct RecF32_dt));
    if (*table_ptr == NULL) return; 

    (*table_ptr)->table = (struct RecF32 *) malloc(((size_t) BASE_DT_SIZE) * sizeof(struct RecF32));
    if ((*table_ptr)->table == NULL) 
    {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    } 
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_RecF32_dt(struct RecF32_dt *table, struct RecF32 *rec)
{
    if(table->element_cnt == table->arr_size)
    {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecF32));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt].data = rec->data;
    table->table[table->element_cnt].key = rec->key;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_RecF32_dt(struct RecF32_dt *table, struct RecF32 *deleted_data)
{
    if(table->element_cnt == 0) return;

    if(deleted_data != NULL)
    {
        deleted_data->data = table->table[table->element_cnt - 1].data;
        deleted_data->key = table->table[table->element_cnt - 1].key;
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE)
    {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecF32));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}




void init_RecF64_dt(struct RecF64_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct RecF64_dt));
    if (*table_ptr == NULL) return; 

    (*table_ptr)->table = (struct RecF64 *) malloc(((size_t) BASE_DT_SIZE) * sizeof(struct RecF64));
    if ((*table_ptr)->table == NULL) 
    {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    } 
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_RecF64_dt(struct RecF64_dt *table, struct RecF64 *rec)
{
    if(table->element_cnt == table->arr_size)
    {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecF64));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt].data = rec->data;
    table->table[table->element_cnt].key = rec->key;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_RecF64_dt(struct RecF64_dt *table, struct RecF64 *deleted_data)
{
    if(table->element_cnt == 0) return;

    if(deleted_data != NULL)
    {
        deleted_data->data = table->table[table->element_cnt - 1].data;
        deleted_data->key = table->table[table->element_cnt - 1].key;
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE)
    {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(struct RecF64));
        if(err_check == NULL)
        {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}


int linear_search_RecI64_dt(struct RecI64_dt *table, int64_t key, size_t *position)
{
    struct RecI64 *begin = table->table;
    struct RecI64 *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(struct RecI64 *ptr = begin; ptr < end; ptr++)
    {
        if(ptr->key == key)
        {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}

int binary_search_RecI64_dt(struct RecI64_dt *table, int64_t key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_reci64(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    struct RecI64 *low = table->table;
    struct RecI64 *high = low + table->element_cnt;
    struct RecI64 *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (mid->key < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && low->key == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

int linear_search_RecU32_dt(struct RecU32_dt *table, uint32_t key, size_t *position)
{
    struct RecU32 *begin = table->table;
    struct RecU32 *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(struct RecU32 *ptr = begin; ptr < end; ptr++)
    {
        if(ptr->key == key)
        {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}

int binary_search_RecU32_dt(struct RecU32_dt *table, uint32_t key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_recu32(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    struct RecU32 *low = table->table;
    struct RecU32 *high = low + table->element_cnt;
    struct RecU32 *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (mid->key < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && low->key == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

int linear_search_RecU64_dt(struct RecU64_dt *table, uint64_t key, size_t *position)
{
    struct RecU64 *begin = table->table;
    struct RecU64 *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(struct RecU64 *ptr = begin; ptr < end; ptr++)
    {
        if(ptr->key == key)
        {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}

int binary_search_RecU64_dt(struct RecU64_dt *table, uint64_t key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_recu64(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    struct RecU64 *low = table->table;
    struct RecU64 *high = low + table->element_cnt;
    struct RecU64 *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (mid->key < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && low->key == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

int linear_search_RecF32_dt(struct RecF32_dt *table, float key, size_t *position)
{
    struct RecF32 *begin = table->table;
    struct RecF32 *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(struct RecF32 *ptr = begin; ptr < end; ptr++)
    {
        if(ptr->key == key)
        {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}

int binary_search_RecF32_dt(struct RecF32_dt *table, float key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_recf32(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    struct RecF32 *low = table->table;
    struct RecF32 *high = low + table->element_cnt;
    struct RecF32 *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (mid->key < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && low->key == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

int linear_search_RecF64_dt(struct RecF64_dt *table, double key, size_t *position)
{
    struct RecF64 *begin = table->table;
    struct RecF64 *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(struct RecF64 *ptr = begin; ptr < end; ptr++)
    {
        if(ptr->key == key)
        {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}

int binary_search_RecF64_dt(struct RecF64_dt *table, double key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_recf64(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    struct RecF64 *low = table->table;
    struct RecF64 *high = low + table->element_cnt;
    struct RecF64 *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (mid->key < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && low->key == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

void init_I32_dt(struct I32_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct I32_dt));
    if (*table_ptr == NULL) return;
    (*table_ptr)->table = (int32_t *) malloc(((size_t) BASE_DT_SIZE) * sizeof(int32_t));
    if ((*table_ptr)->table == NULL) {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    }
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_I32_dt(struct I32_dt *table, int32_t val)
{
    if(table->element_cnt == table->arr_size) {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(int32_t));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt] = val;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_I32_dt(struct I32_dt *table, int32_t *deleted_data)
{
    if(table->element_cnt == 0) return;
    if(deleted_data != NULL) {
        *deleted_data = table->table[table->element_cnt - 1];
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE) {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(int32_t));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}

int linear_search_I32_dt(struct I32_dt *table, int32_t key, size_t *position)
{
    int32_t *begin = table->table;
    int32_t *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(int32_t *ptr = begin; ptr < end; ptr++) {
        if(*ptr == key) {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}

int binary_search_I32_dt(struct I32_dt *table, int32_t key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_int32(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    int32_t *low = table->table;
    int32_t *high = low + table->element_cnt;
    int32_t *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (*mid < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && *low == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}


void init_I64_dt(struct I64_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct I64_dt));
    if (*table_ptr == NULL) return;
    (*table_ptr)->table = (int64_t *) malloc(((size_t) BASE_DT_SIZE) * sizeof(int64_t));
    if ((*table_ptr)->table == NULL) {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    }
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_I64_dt(struct I64_dt *table, int64_t val)
{
    if(table->element_cnt == table->arr_size) {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(int64_t));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt] = val;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_I64_dt(struct I64_dt *table, int64_t *deleted_data)
{
    if(table->element_cnt == 0) return;
    if(deleted_data != NULL) {
        *deleted_data = table->table[table->element_cnt - 1];
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE) {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(int64_t));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}

int linear_search_I64_dt(struct I64_dt *table, int64_t key, size_t *position)
{
    int64_t *begin = table->table;
    int64_t *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for( int64_t *ptr = begin; ptr < end; ptr++) {
        if( *ptr == key) {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}

int binary_search_I64_dt(struct I64_dt *table, int64_t key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_int64(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    int64_t *low = table->table;
    int64_t *high = low + table->element_cnt;
    int64_t *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (*mid < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && *low == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

void init_U32_dt(struct U32_dt **table_ptr)
{
    *table_ptr = malloc(sizeof(struct U32_dt));
    if (*table_ptr == NULL) return;
    (*table_ptr)->table = (uint32_t *) malloc(((size_t) BASE_DT_SIZE) * sizeof(uint32_t));
    if ((*table_ptr)->table == NULL) {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    }
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_U32_dt(struct U32_dt *table, uint32_t val)
{
    if(table->element_cnt == table->arr_size) {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(uint32_t));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt] = val;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_U32_dt(struct U32_dt *table, uint32_t *deleted_data) {
    if(table->element_cnt == 0) return;
    if(deleted_data != NULL) {
        *deleted_data = table->table[table->element_cnt - 1];
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE) {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(uint32_t));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}

int linear_search_U32_dt(struct U32_dt *table, uint32_t key, size_t *position) {
    uint32_t *begin = table->table;
    uint32_t *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(uint32_t *ptr = begin; ptr < end; ptr++) {
        if(*ptr == key) {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;
}

int binary_search_U32_dt(struct U32_dt *table, uint32_t key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_uint32(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    uint32_t *low = table->table;
    uint32_t *high = low + table->element_cnt;
    uint32_t *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (*mid < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && *low == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

void init_U64_dt(struct U64_dt **table_ptr) {
    *table_ptr = malloc(sizeof(struct U64_dt));
    if (*table_ptr == NULL) return;
    (*table_ptr)->table = (uint64_t *) malloc(((size_t) BASE_DT_SIZE) * sizeof(uint64_t));
    if ((*table_ptr)->table == NULL) {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    }
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_U64_dt(struct U64_dt *table, uint64_t val) {
    if(table->element_cnt == table->arr_size) {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(uint64_t));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt] = val;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_U64_dt(struct U64_dt *table, uint64_t *deleted_data) {
    if(table->element_cnt == 0) return;
    if(deleted_data != NULL) {
        *deleted_data = table->table[table->element_cnt - 1];
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE) {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(uint64_t));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}

int linear_search_U64_dt(struct U64_dt *table, uint64_t key, size_t *position) {
    uint64_t *begin = table->table;
    uint64_t *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(uint64_t *ptr = begin; ptr < end; ptr++) {
        if(*ptr == key) {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;   
}

int binary_search_U64_dt(struct U64_dt *table, uint64_t key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_uint64(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    uint64_t *low = table->table;
    uint64_t *high = low + table->element_cnt;
    uint64_t *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (*mid < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && *low == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

void init_F32_dt(struct F32_dt **table_ptr) {
    *table_ptr = malloc(sizeof(struct F32_dt));
    if (*table_ptr == NULL) return;
    (*table_ptr)->table = (float *) malloc(((size_t) BASE_DT_SIZE) * sizeof(float));
    if ((*table_ptr)->table == NULL) {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    }
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_F32_dt(struct F32_dt *table, float val) {
    if(table->element_cnt == table->arr_size) {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(float));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt] = val;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_F32_dt(struct F32_dt *table, float *deleted_data) {
    if(table->element_cnt == 0) return;
    if(deleted_data != NULL) {
        *deleted_data = table->table[table->element_cnt - 1];
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE) {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(float));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}

int linear_search_F32_dt(struct F32_dt *table, float key, size_t *position) {
    float *begin = table->table;
    float *end = table->table + table->element_cnt;
#pragma GCC unroll 4
    for(float *ptr = begin; ptr < end; ptr++) {
        if(*ptr == key) {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;   

}

int binary_search_F32_dt(struct F32_dt *table, float key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_float(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    float *low = table->table;
    float *high = low + table->element_cnt;
    float *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (*mid < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && *low == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}

void init_F64_dt(struct F64_dt **table_ptr) {
    *table_ptr = malloc(sizeof(struct F64_dt));
    if (*table_ptr == NULL) return;
    (*table_ptr)->table = (double *) malloc(((size_t) BASE_DT_SIZE) * sizeof(double));
    if ((*table_ptr)->table == NULL) {
        free(*table_ptr);
        *table_ptr = NULL;
        return;
    }
    (*table_ptr)->element_cnt = 0;
    (*table_ptr)->arr_size = (size_t) BASE_DT_SIZE;
    (*table_ptr)->is_sorted = 1;
}

void insert_F64_dt(struct F64_dt *table, double val) {
    if(table->element_cnt == table->arr_size) {
        size_t new_size = table->arr_size * 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(double));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
    table->table[table->element_cnt] = val;
    table->element_cnt++;
    table->is_sorted = 0;
}

void delete_F64_dt(struct F64_dt *table, double *deleted_data) {
    if(table->element_cnt == 0) return;
    if(deleted_data != NULL) {
        *deleted_data = table->table[table->element_cnt - 1];
    }
    table->element_cnt--;
    if(table->element_cnt == table->arr_size / 4 && table->arr_size != BASE_DT_SIZE) {
        size_t new_size = table->arr_size / 2;
        void *err_check = reallocarray(table->table, new_size, sizeof(double));
        if(err_check == NULL) {
            printf("reallocation failed...\n");
            return;
        }
        table->arr_size = new_size;
        table->table = err_check;
    }
}

int linear_search_F64_dt(struct F64_dt *table, double key, size_t *position)
{
    double *begin = table->table;
    double *end = table->table + table->element_cnt;
#pragma GCC unroll 4 
    for(double *ptr = begin; ptr < end; ptr++) {
        if(*ptr == key) {
            *position = (size_t)(ptr - begin);
            return 1;
        }
    }
    return 0;   
}

int binary_search_F64_dt(struct F64_dt *table, double key, size_t *position)
{
    if(!table->is_sorted)
    {
        universal_sort_double(table->table, table->element_cnt);
        table->is_sorted = 1;
    }
    double *low = table->table;
    double *high = low + table->element_cnt;
    double *mid;
    while (low < high)
    {
        mid = low + ((high - low) >> 1);

        if (*mid < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    if (low < table->table + table->element_cnt && *low == key)
    {
        // Subtracting the base pointer gives the correct size_t index
        *position = (size_t)(low - table->table);
        return 1;
    }
    return 0;
}


void free_RecI32_dt(struct RecI32_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_RecI64_dt(struct RecI64_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_RecU32_dt(struct RecU32_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_RecU64_dt(struct RecU64_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_RecF32_dt(struct RecF32_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_RecF64_dt(struct RecF64_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}


void free_I32_dt(struct I32_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_I64_dt(struct I64_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_U32_dt(struct U32_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_U64_dt(struct U64_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_F32_dt(struct F32_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}

void free_F64_dt(struct F64_dt **table_ptr) {
    if (table_ptr != NULL && *table_ptr != NULL) {
        free((*table_ptr)->table);
        free(*table_ptr);
        *table_ptr = NULL;
    }
}
