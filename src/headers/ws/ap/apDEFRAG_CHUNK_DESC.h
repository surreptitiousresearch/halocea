#pragma once
// ws-engine ap: descriptor shared by every chunk of one logical allocation kind.
// Holds the make/copy/relocate callbacks and back-links to the owning pool.
// DB-verified layout (types_members apDEFRAG_CHUNK_DESC):
//   fnMake@0, fnCopy@4, callback@8, pool@12, id@16 (dsTSTRING<char>) — size 20.
#include "../ds/dsTSTRING.h"

struct apDEFRAG_CHUNK;
struct apDEFRAG_POOL;

typedef struct apDEFRAG_CHUNK_DESC {
    apDEFRAG_CHUNK *(*fnMake)(const char *dbg, int size);                     // 0x00 factory
    void            (*fnCopy)(apDEFRAG_CHUNK *dst, apDEFRAG_CHUNK *src); // 0x04
    void            (*callback)(apDEFRAG_CHUNK *chunk);                       // 0x08 post-move notify
    apDEFRAG_POOL  *pool;                                                     // 0x0C owning pool
    dsTSTRING<char> id;                                                       // 0x10 debug name
} apDEFRAG_CHUNK_DESC;
