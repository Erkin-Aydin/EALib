#ifndef RECORD_H
#define RECORD_H

//#include <stdlib.h>
#include <stdint.h>

/* ========================================================================= *
 * DATA STRUCTURES (Universal Key-Value Pairs)                               *
 * ========================================================================= */

struct RecU32 { uint32_t key; void* data; };
struct RecI32 { int32_t  key; void* data; };
struct RecF32 { float    key; void* data; };
struct RecU64 { uint64_t key; void* data; };
struct RecI64 { int64_t  key; void* data; };
struct RecF64 { double   key; void* data; };

#endif
