#pragma once
/* apDEFRAG_CHUNK — ws-engine defrag-managed memory chunk (boundary type), 28-byte DB layout.
 * A locked chunk hands back a live pointer through the vtable Lock/Unlock methods; `ptr` tracks
 * the current write cursor and `pBuf->ptr` is the owning buffer's base, so `ptr - pBuf->ptr`
 * is the byte offset written so far. Layout + vtable slots verified against types_members. */

#include "apDEFRAG_BUF.h"
#include "ws/ap/apSTATE_T.h"

typedef struct apDEFRAG_CHUNK       apDEFRAG_CHUNK;
typedef struct apDEFRAG_CHUNK_DESC  apDEFRAG_CHUNK_DESC;

typedef struct apDEFRAG_CHUNK_vtbl
{
    const char *(*DbgInfo)(apDEFRAG_CHUNK *self);                    /* 0x00 */
    void       *(*LockRead)(apDEFRAG_CHUNK *self);                   /* 0x04 */
    void       *(*LockWrite)(apDEFRAG_CHUNK *self);                  /* 0x08 */
    void        (*Unlock)(apDEFRAG_CHUNK *self);                     /* 0x0C */
    int         (*Alignment)(apDEFRAG_CHUNK *self);                  /* 0x10 */
    int         (*MemMove)(apDEFRAG_CHUNK *self, void *, void *, int, bool); /* 0x14 */
    /* scalar-deleting dtor slot @0x18. Two reconstructions named this slot differently
     * (`dtr` here, `dtr_apDEFRAG_CHUNK` in the ws/ap variant); expose both names for the same
     * single slot so all consumers compile (layout-neutral: one function pointer). */
    union {
        void    (*dtr)(apDEFRAG_CHUNK *self, int deleteFlag);
        void    (*dtr_apDEFRAG_CHUNK)(apDEFRAG_CHUNK *self, int deleteFlag);
    };
    const char *(*Id)(apDEFRAG_CHUNK *self);                         /* 0x1C */
} apDEFRAG_CHUNK_vtbl;

struct apDEFRAG_CHUNK
{
    apDEFRAG_CHUNK_vtbl *__vftable;  /* 0x00 */
    apDEFRAG_CHUNK_DESC *pDesc;      /* 0x04 */
    apDEFRAG_BUF        *pBuf;       /* 0x08 owning buffer */
    char               *ptr;        /* 0x0C current write cursor */
    int                 size;       /* 0x10 */
    char               *ptrPrev;    /* 0x14 */
    apSTATE_T<unsigned char> state; /* 0x18 */
    unsigned char       _pad19[3];  /* 0x19 */
};                                   /* 28 bytes */
