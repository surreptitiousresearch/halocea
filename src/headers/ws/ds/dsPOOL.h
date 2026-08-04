#pragma once
#include "../../apCL.h"
// ws-engine ds: chunked free-list object pool. Elements live in heap-allocated chunks of
// CHUNK_ELEMS each; up to CHUNK_COUNT chunk slots are tracked inline. Free slots are threaded
// through a singly-linked free list (each free element stores the next free index).
// DB-verified layout (types_members dsPOOL<char const*,1024,512>):
//   numChunks@0, emptyChain@4, chunkTable@8 (CHUNK_REC[CHUNK_COUNT]), cl@(8+4*CHUNK_COUNT).
// Template params: T = element type; CHUNK_ELEMS = elements per chunk; CHUNK_COUNT = chunk slots.

template<class T, int CHUNK_ELEMS, int CHUNK_COUNT>
struct dsPOOL {
    // One chunk-table slot: a pointer to the chunk's element array (null until allocated).
    struct CHUNK_REC {
        T *data; // 0x00
    };

    int       numChunks;               // 0x00 number of allocated chunks
    int       emptyChain;              // 0x04 head index of the free list (-1 = empty)
    CHUNK_REC chunkTable[CHUNK_COUNT];  // 0x08 chunk pointer table
    apCL      cl;                      // allocation call-site cookie

    // 0x82529268 (dsPOOL<char const*,1024,512>) — take a free slot (growing the pool if needed),
    // store `value` into it, and return its index.
    int Alloc(const T &value);

    // 0x82614998 / 0x82614CE0 / 0x82615E10 — no-argument form used by the event/state managers:
    // take the head free slot (growing the pool if the free list is empty), unlink it, and return
    // its index. The freed slot's first word aliases the "next free index" free-list link.
    int Alloc();

    // Allocate and thread a fresh chunk of CHUNK_ELEMS elements onto the free list; not decompiled.
    void AllocChunk(); // boundary

    // 0x825283B8 (dsPOOL<char const*,1024,512>) etc. — non-const accessor: resolve a slot index
    // to a reference to its stored element (chunk idx/CHUNK_ELEMS, position idx%CHUNK_ELEMS).
    // Asserts the index is in range first.
    T &Get(int index);

    // 0x82529300 (dsPOOL<char const*,1024,512>) — const accessor: resolve a slot index to a
    // reference to its stored element (chunk idx/CHUNK_ELEMS, position idx%CHUNK_ELEMS). Asserts
    // the index is in range first.
    const T &Get(int index) const;
};
