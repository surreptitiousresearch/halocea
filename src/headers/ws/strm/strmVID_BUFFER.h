#pragma once
#include "../ap/apSTATE_T.h"

/* strmVID_BUFFER — ws-engine strm subsystem: one sub-allocation ("buffer") carved out of a
 * strmVID_MEM_BLOCK's physical-memory reservation. DB-verified layout (types_members
 * strmVID_BUFFER), size 0x28 (40) — confirmed by the sizeof(strmVID_BUFFER) argument to
 * operator new in strmVID_MEM_BLOCK::AddNewBuffer. */

struct strmVID_MEM_BLOCK; // ws/strm/strmVID_MEM_BLOCK.h
struct strmVID_MEMMOVE_CB; // boundary — async-compaction move-notify callback, not decompiled

typedef struct strmVID_BUFFER
{
    unsigned int          id;         /* 0x00 buffer id (monotonic counter, see strm::bufId) */
    strmVID_MEM_BLOCK     *block;     /* 0x04 owning block */
    apSTATE_T<unsigned char> state;   /* 0x08 buffer state byte */
    unsigned char _pad0[3]; /* db-verified padding */
    unsigned char          *offset;   /* 0x0C pointer into the block's physical-memory range */
    unsigned int            size;     /* 0x10 */
    unsigned int            align;    /* 0x14 */
    strmVID_BUFFER          *prev;    /* 0x18 sibling links (address-ordered free/alloc chain) */
    strmVID_BUFFER          *next;    /* 0x1C */
    int                      waitFrames; /* 0x20 frames remaining before an async op is safe */
    strmVID_MEMMOVE_CB      *memmoveCb; /* 0x24 async-compaction move-notify callback, if any */
} strmVID_BUFFER;
