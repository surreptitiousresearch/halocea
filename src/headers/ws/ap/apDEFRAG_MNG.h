#pragma once
// ws-engine ap: defragmenting allocation manager — owns the set of defrag pools and
// routes chunk alloc/free through the matching descriptor and pool.
// DB-verified layout (types_members apDEFRAG_MNG): pools@0 (dsVECTOR<apDEFRAG_POOL*,8>) — size 20.
#include "../ds/dsVECTOR.h"
#include "apDEFRAG_CHUNK.h"
#include "apDEFRAG_CHUNK_DESC.h"
#include "apDEFRAG_POOL.h"

typedef struct apDEFRAG_MNG {
    dsVECTOR<apDEFRAG_POOL *, 8> pools; // 0x00 registered pools

    // Detach `chunk` from its buffer and destroy it.
    void Free(apDEFRAG_CHUNK *chunk);

    // Bind `chunk` to its descriptor, set its size, and place it into the pool.
    // Returns the chunk on success, nullptr if no descriptor matched.
    apDEFRAG_CHUNK *AllocImpl(apDEFRAG_CHUNK *chunk, int size);

    // Allocate a chunk of concrete type T (new T() + AllocImpl); nullptr on failure. Defined
    // out-of-line in apDEFRAG_MNG__Alloc.cpp; instantiated per chunk kind (vidDEFRAG_CHUNK_TEX,
    // vidDEFRAG_CHUNK_OBJ_X360, vidDEFRAG_CHUNK_OBJ_HCEX_X360, ...).
    template<class T> T *Alloc(int size);

    // 0x827B3C80 — look up the descriptor governing `chunk`: scan every pool's descriptor list for
    // the one whose id string equals chunk->Id(); returns it, or nullptr. const (ABA mangle).
    apDEFRAG_CHUNK_DESC *FindDesc(apDEFRAG_CHUNK *chunk) const;

    // 0x827B44F0 — log per-pool / per-buffer used/free block statistics to the ap log (and, when
    // `filename` is non-null, force-log the same plus every chunk's debug info to that file).
    void DumpStat(const char *filename);

    // Construct a POOL and push it onto `pools` (boundary — not decompiled for every instantiation).
    template<class POOL> void RegisterPool();
    // Register a chunk-descriptor kind under `poolId`, with an optional per-chunk callback
    // (boundary — not decompiled for every instantiation).
    template<class CHUNK> void RegisterChunk(const char *poolId, void (__fastcall *callback)(apDEFRAG_CHUNK *));
    // Reserve/allocate the backing buffer for the named pool.
    void AllocBuf(const char *poolId, int size);
} apDEFRAG_MNG;
