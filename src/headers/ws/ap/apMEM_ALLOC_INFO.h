#pragma once
// ws-engine ap: one small-allocation statistics record kept in the apMEM_STAT_HASH table
// (and, for allocator blocks, mirrored in the tail footer of the dl chunk).
// DB-verified layout (types_members apMEM_ALLOC_INFO):
//   fileName@0, memSize@4, memBlock@8, fileLine@12 (u16), type@14 (char) — size 16.

typedef struct apMEM_ALLOC_INFO {
    const char    *fileName; // 0x00 source file of the allocation
    unsigned int   memSize;  // 0x04 requested byte size
    void          *memBlock; // 0x08 allocated block pointer (0 = vacant slot)
    unsigned short fileLine; // 0x0C source line
    char           type;     // 0x0E apMEM_BLOCK_TYPE, stored narrowed to a byte
} apMEM_ALLOC_INFO;
