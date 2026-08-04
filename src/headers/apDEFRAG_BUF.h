#pragma once
/* apDEFRAG_BUF — ws-engine defragmenting GPU-memory buffer (boundary type). 96-byte DB layout.
 * One contiguous backing buffer of a defrag pool: owns the chunks placed inside it plus the
 * free/used block bookkeeping used while relocating them. HCEX only reads `ptr` (the buffer
 * base) to compute a chunk's write offset. Layout DB-verified against types_members.
 * The block records are nested classes in the DB (types_members apDEFRAG_BUF::MEM_BLOCK,
 * types_members apDEFRAG_BUF::MEM_BLOCK_USED); spelled here at namespace scope so the
 * verifier can resolve them — namespace qualification only, same layouts. */

#include "ws/ap/apSTATE_T.h"
#include "ws/ds/dsVECTOR.h"

typedef struct apDEFRAG_POOL      apDEFRAG_POOL;
struct apDEFRAG_CHUNK;
struct apDEFRAG_BUF;

// vtable — DB-verified (types_members apDEFRAG_BUF_vtbl). Full body given here (was a bare forward
// typedef) so consumers that pick up this flat header can dispatch through the slots.
typedef struct apDEFRAG_BUF_vtbl {
    void  (__fastcall *RemoveChunk)(apDEFRAG_BUF *self, apDEFRAG_CHUNK *chunk);
    char *(__fastcall *FitIntoFreeBlock)(apDEFRAG_BUF *self, apDEFRAG_CHUNK *chunk, int align);
    bool  (__fastcall *MoveChunk)(apDEFRAG_BUF *self, int block, apDEFRAG_CHUNK *chunk);
    void  (__fastcall *DefragAllInside)(apDEFRAG_BUF *self);
    void  (__fastcall *dtr_apDEFRAG_BUF)(apDEFRAG_BUF *self);
} apDEFRAG_BUF_vtbl;

/* free/reclaimable span inside the buffer. DB types_members apDEFRAG_BUF::MEM_BLOCK. */
typedef struct MEM_BLOCK {
    apSTATE_T<unsigned char> state;  /* 0x00 */
    unsigned char           _pad1[3];/* 0x01 */
    int                      offset; /* 0x04 byte offset from buffer base */
    int                      size;   /* 0x08 span length */
} MEM_BLOCK;                                   /* 12 bytes */

/* a span still occupied by a chunk mid-relocation (leading MEM_BLOCK inherited at offset 0).
 * DB types_members apDEFRAG_BUF::MEM_BLOCK_USED. */
struct MEM_BLOCK_USED : MEM_BLOCK {
    int             nFramesToWait; /* 0x0C defer relocation until this reaches 0 */
    int             newBuf;        /* 0x10 destination buffer index */
    char           *newPtr;        /* 0x14 destination pointer */
    apDEFRAG_CHUNK *pChunk;        /* 0x18 chunk being moved */
};                                 /* 28 bytes */

typedef struct apDEFRAG_BUF
{
    apDEFRAG_BUF_vtbl               *__vftable;  /* 0x00 */
    char                            *ptr;        /* 0x04 buffer base */
    int                              size;       /* 0x08 buffer byte size */
    apDEFRAG_POOL                   *pool;       /* 0x0C owning pool */
    dsVECTOR<apDEFRAG_CHUNK *, 8>    chunks;     /* 0x10 live chunks */
    dsVECTOR<MEM_BLOCK, 8>           freeBlocks; /* 0x24 free spans */
    dsVECTOR<apDEFRAG_CHUNK *, 8>    chunksRT;   /* 0x38 runtime-tracked chunks */
    dsVECTOR<MEM_BLOCK_USED, 8>      usedBlocks; /* 0x4C in-flight relocations */

    /* Base implementations behind the vtable slots (boundary — not reversed here). Declared as
     * members so derived thunks (e.g. vidDEFRAG_BUF_HCEX_X360::RemoveChunk) can call the base
     * form apDEFRAG_BUF::RemoveChunk(...). No layout impact (non-virtual member decls). */
    void RemoveChunk(apDEFRAG_CHUNK *chunk);
} apDEFRAG_BUF;                                               /* 96 bytes */
