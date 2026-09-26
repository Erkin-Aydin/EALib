#ifndef _DYNAMIC_TABLE_H_
#define _DYNAMIC_TABLE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"

#define BASE_DT_SIZE 8

struct RecI32_dt { size_t arr_size; size_t element_cnt; int is_sorted; struct RecI32 *table; };
struct RecI64_dt { size_t arr_size; size_t element_cnt; int is_sorted; struct RecI64 *table; };
struct RecU32_dt { size_t arr_size; size_t element_cnt; int is_sorted; struct RecU32 *table; };
struct RecU64_dt { size_t arr_size; size_t element_cnt; int is_sorted; struct RecU64 *table; };
struct RecF32_dt { size_t arr_size; size_t element_cnt; int is_sorted; struct RecF32 *table; };
struct RecF64_dt { size_t arr_size; size_t element_cnt; int is_sorted; struct RecF64 *table; };

struct I32_dt { size_t arr_size; size_t element_cnt; int is_sorted; int32_t *table; };
struct I64_dt { size_t arr_size; size_t element_cnt; int is_sorted; int64_t *table; };
struct U32_dt { size_t arr_size; size_t element_cnt; int is_sorted; uint32_t *table; };
struct U64_dt { size_t arr_size; size_t element_cnt; int is_sorted; uint64_t *table; };
struct F32_dt { size_t arr_size; size_t element_cnt; int is_sorted; float *table; };
struct F64_dt { size_t arr_size; size_t element_cnt; int is_sorted; double *table; };



void init_RecI32_dt(struct RecI32_dt **table_ptr);
void init_RecI64_dt(struct RecI64_dt **table_ptr);
void init_RecU32_dt(struct RecU32_dt **table_ptr);
void init_RecU64_dt(struct RecU64_dt **table_ptr);
void init_RecF32_dt(struct RecF32_dt **table_ptr);
void init_RecF64_dt(struct RecF64_dt **table_ptr);

void insert_RecI32_dt(struct RecI32_dt *table, struct RecI32 *rec);
void insert_RecI64_dt(struct RecI64_dt *table, struct RecI64 *rec);
void insert_RecU32_dt(struct RecU32_dt *table, struct RecU32 *rec);
void insert_RecU64_dt(struct RecU64_dt *table, struct RecU64 *rec);
void insert_RecF32_dt(struct RecF32_dt *table, struct RecF32 *rec);
void insert_RecF64_dt(struct RecF64_dt *table, struct RecF64 *rec);

void delete_RecI32_dt(struct RecI32_dt *table, struct RecI32 *deleted_data);
void delete_RecI64_dt(struct RecI64_dt *table, struct RecI64 *deleted_data);
void delete_RecU32_dt(struct RecU32_dt *table, struct RecU32 *deleted_data);
void delete_RecU64_dt(struct RecU64_dt *table, struct RecU64 *deleted_data);
void delete_RecF32_dt(struct RecF32_dt *table, struct RecF32 *deleted_data);
void delete_RecF64_dt(struct RecF64_dt *table, struct RecF64 *deleted_data);

int linear_search_RecI32_dt(struct RecI32_dt *table, int32_t key, size_t *position);
int binary_search_RecI32_dt(struct RecI32_dt *table, int32_t key, size_t *position);

int linear_search_RecI64_dt(struct RecI64_dt *table, int64_t key, size_t *position);
int binary_search_RecI64_dt(struct RecI64_dt *table, int64_t key, size_t *position);

int linear_search_RecU32_dt(struct RecU32_dt *table, uint32_t key, size_t *position);
int binary_search_RecU32_dt(struct RecU32_dt *table, uint32_t key, size_t *position);

int linear_search_RecU64_dt(struct RecU64_dt *table, uint64_t key, size_t *position);
int binary_search_RecU64_dt(struct RecU64_dt *table, uint64_t key, size_t *position);

int linear_search_RecF32_dt(struct RecF32_dt *table, float key, size_t *position);
int binary_search_RecF32_dt(struct RecF32_dt *table, float key, size_t *position);

int linear_search_RecF64_dt(struct RecF64_dt *table, double key, size_t *position);
int binary_search_RecF64_dt(struct RecF64_dt *table, double key, size_t *position);




void init_I32_dt(struct I32_dt **table_ptr);
void insert_I32_dt(struct I32_dt *table, int32_t val);
void delete_I32_dt(struct I32_dt *table, int32_t *deleted_data);
int linear_search_I32_dt(struct I32_dt *table, int32_t key, size_t *position);
int binary_search_I32_dt(struct I32_dt *table, int32_t key, size_t *position);


void init_I64_dt(struct I64_dt **table_ptr);
void insert_I64_dt(struct I64_dt *table, int64_t val);
void delete_I64_dt(struct I64_dt *table, int64_t *deleted_data);
int linear_search_I64_dt(struct I64_dt *table, int64_t key, size_t *position);
int binary_search_I64_dt(struct I64_dt *table, int64_t key, size_t *position);


void init_U32_dt(struct U32_dt **table_ptr);
void insert_U32_dt(struct U32_dt *table, uint32_t val);
void delete_U32_dt(struct U32_dt *table, uint32_t *deleted_data);
int linear_search_U32_dt(struct U32_dt *table, uint32_t key, size_t *position);
int binary_search_U32_dt(struct U32_dt *table, uint32_t key, size_t *position);


void init_U64_dt(struct U64_dt **table_ptr);
void insert_U64_dt(struct U64_dt *table, uint64_t val);
void delete_U64_dt(struct U64_dt *table, uint64_t *deleted_data);
int linear_search_U64_dt(struct U64_dt *table, uint64_t key, size_t *position);
int binary_search_U64_dt(struct U64_dt *table, uint64_t key, size_t *position);


void init_F32_dt(struct F32_dt **table_ptr);
void insert_F32_dt(struct F32_dt *table, float val);
void delete_F32_dt(struct F32_dt *table, float *deleted_data);
int linear_search_F32_dt(struct F32_dt *table, float key, size_t *position);
int binary_search_F32_dt(struct F32_dt *table, float key, size_t *position);


void init_F64_dt(struct F64_dt **table_ptr);
void insert_F64_dt(struct F64_dt *table, double val);
void delete_F64_dt(struct F64_dt *table, double *deleted_data);
int linear_search_F64_dt(struct F64_dt *table, double key, size_t *position);
int binary_search_F64_dt(struct F64_dt *table, double key, size_t *position);

void free_RecI32_dt(struct RecI32_dt **table_ptr);
void free_RecI64_dt(struct RecI64_dt **table_ptr);
void free_RecU32_dt(struct RecU32_dt **table_ptr);
void free_RecU64_dt(struct RecU64_dt **table_ptr);
void free_RecF32_dt(struct RecF32_dt **table_ptr);
void free_RecF64_dt(struct RecF64_dt **table_ptr);

void free_I32_dt(struct I32_dt **table_ptr);
void free_I64_dt(struct I64_dt **table_ptr);
void free_U32_dt(struct U32_dt **table_ptr);
void free_U64_dt(struct U64_dt **table_ptr);
void free_F32_dt(struct F32_dt **table_ptr);
void free_F64_dt(struct F64_dt **table_ptr);

#endif 
