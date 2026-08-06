#pragma once
// ws-engine ap: a defrag pool — a set of backing buffers plus the chunk descriptors
// registered against them. FitChunk places a chunk into a buffer with room for it.
// DB-verified layout (types_members apDEFRAG_POOL):
//   __vftable@0, buffers@4, descs@24 — size 44.
#include "../ds/dsVECTOR.h"

struct apDEFRAG_POOL;
struct apDEFRAG_BUF;
struct apDEFRAG_CHUNK;
struct apDEFRAG_CHUNK_DESC;

// vtable — DB-verified (types_members apDEFRAG_POOL_vtbl).
typedef struct apDEFRAG_POOL_vtbl {
    void          (*dtr_apDEFRAG_POOL)(apDEFRAG_POOL *self);
    void         *(*AllocBuf)(apDEFRAG_POOL *self, int size, int *outIdx);
    bool          (*FreeBuf)(apDEFRAG_POOL *self, apDEFRAG_BUF *buf);
    const char   *(*Id)(apDEFRAG_POOL *self);
    apDEFRAG_BUF *(*CreateBuf)(apDEFRAG_POOL *self, char *base, int size);
    void          (*FitChunk)(apDEFRAG_POOL *self, apDEFRAG_CHUNK *chunk);
    void          (*DumpStat)(apDEFRAG_POOL *self);
} apDEFRAG_POOL_vtbl;

typedef struct apDEFRAG_POOL {
    apDEFRAG_POOL_vtbl                  *__vftable; // 0x00
    dsVECTOR<apDEFRAG_BUF *, 8>          buffers;   // 0x04
    dsVECTOR<apDEFRAG_CHUNK_DESC *, 8>   descs;     // 0x18
} apDEFRAG_POOL;
