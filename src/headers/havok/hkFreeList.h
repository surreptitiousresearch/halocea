#pragma once

/* hkFreeList (size 48, DB types_members) — a pooled fixed-size-element allocator.
   Free elements are singly linked through m_free; fresh elements are bump-allocated
   from [m_top, m_blockEnd) until a block is exhausted, at which point addSpace()
   grabs another block from the memory server. */

typedef struct hkFreeListMemoryServer hkFreeListMemoryServer; /* boundary */

typedef struct hkFreeList_Element
{
    struct hkFreeList_Element *m_next; /* 0x00 */
} hkFreeList_Element;

typedef struct hkFreeList_Block hkFreeList_Block; /* boundary — block header */

typedef struct hkFreeList
{
    hkFreeList_Element *m_free;             /* 0x00 */
    unsigned int m_elementSize;             /* 0x04 */
    hkFreeList_Block *m_blocks;             /* 0x08 */
    hkFreeList_Block *m_freeBlocks;         /* 0x0C */
    unsigned int m_blockSize;               /* 0x10 */
    unsigned int m_align;                   /* 0x14 */
    unsigned int m_maxBlockSize;            /* 0x18 */
    unsigned char *m_top;                   /* 0x1C */
    unsigned char *m_blockEnd;              /* 0x20 */
    unsigned int m_numFreeElements;         /* 0x24 */
    unsigned int m_totalNumElements;        /* 0x28 */
    hkFreeListMemoryServer *m_memoryServer; /* 0x2C */
} hkFreeList;

/* boundary ops (SDK floor). */
void hkFreeList_construct(hkFreeList *self, unsigned int elementSize, unsigned int align,
                          unsigned int blockSize, hkFreeListMemoryServer *server); /* hkFreeList::hkFreeList */
void *hkFreeList_addSpace(hkFreeList *self);                                       /* hkFreeList::addSpace */
void hkFreeList_freeAllMemory(hkFreeList *self);                                   /* hkFreeList::freeAllMemory */

/* Inline element allocation reproduced from the call site: pop the free list, else
   bump-allocate from the current block, else grow via addSpace(). */
static void *hkFreeList_alloc(hkFreeList *self)
{
    if (self->m_free)
    {
        void *p = self->m_free;
        --self->m_numFreeElements;
        self->m_free = self->m_free->m_next;
        return p;
    }
    if (self->m_top < self->m_blockEnd)
    {
        void *p = self->m_top;
        --self->m_numFreeElements;
        self->m_top += self->m_elementSize;
        return p;
    }
    return hkFreeList_addSpace(self);
}

/* Default free-list memory server singleton accessor (boundary). */
hkFreeListMemoryServer *hkDefaultFreeListMemoryServer_getInstance(void);
