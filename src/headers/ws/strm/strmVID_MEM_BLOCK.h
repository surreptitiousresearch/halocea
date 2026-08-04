#pragma once
#include "../ds/dsVECTOR.h"
#include "strmVID_BUFFER.h"

/* strmVID_MEM_BLOCK — ws-engine strm subsystem: one physical-memory reservation (a chunk of GPU/
 * video memory obtained from vidAllocPhysMem) sub-allocated into strmVID_BUFFER entries. DB-verified
 * layout (types_members strmVID_MEM_BLOCK), size 0x48 (72) — confirmed by the sizeof(strmVID_MEM_BLOCK)
 * argument to operator new in strmVID_MEM_MANAGER::ReserveAllMemory. */

struct vidDEFRAG_CHUNK; // boundary — vid subsystem defrag-managed chunk handle

typedef struct strmVID_MEM_BLOCK
{
    int                          size;        /* 0x00 total bytes reserved */
    unsigned char                *offset;     /* 0x04 base pointer into physical memory */
    vidDEFRAG_CHUNK               *pChunk;     /* 0x08 defrag-manager chunk handle, if compacted */
    bool                           compacted;  /* 0x0C */
    unsigned char                  _pad0D[3];  /* 0x0D db-verified padding */
    unsigned int                   id;         /* 0x10 block id (monotonic counter, see strm::blockId) */
    unsigned int                   lockFrames; /* 0x14 frames the block is pinned against compaction */
    dsVECTOR<strmVID_BUFFER *, 8>  buffers;         /* 0x18 address-ordered live buffer list */
    dsVECTOR<strmVID_BUFFER *, 8>  asyncFreeBufList; /* 0x2C buffers pending async free */
    int                             freeSize;        /* 0x40 bytes currently free */
    int                             freeBufMaxSize;   /* 0x44 largest single free-buffer size */

    // 0x8269CCA0 — allocate the block's physical-memory reservation (falling back to "whatever is
    // left" on failure), seed the free list with one buffer spanning the whole range, and record id.
    bool Init(unsigned int blockId);

/* private: */
    // 0x8269BDE8 — carve out and link a new strmVID_BUFFER sub-allocation, insert it (address-sorted)
    // into `buffers`, and return it.
    strmVID_BUFFER *AddNewBuffer(unsigned char state, unsigned char *offset, unsigned long size,
                                  unsigned long align, strmVID_BUFFER *prev, strmVID_BUFFER *next);
} strmVID_MEM_BLOCK;
