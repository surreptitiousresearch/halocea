#pragma once

/* Havok per-thread bump allocator (336 bytes, DB types_members). */

typedef struct hkThreadMemory_vtbl hkThreadMemory_vtbl; /* boundary — allocator vtable */
typedef struct hkMemory hkMemory;                       /* boundary — global allocator */

struct hkThreadMemory
{
    /* nested hkThreadMemory::Stack (16 bytes, DB types_members) */
    struct Stack
    {
        char *m_current;               /* 0x00 */
        struct Stack *m_prev;          /* 0x04 */
        char *m_base;                  /* 0x08 */
        char *m_end;                   /* 0x0C */
    };
    /* nested hkThreadMemory::FreeList (8 bytes, DB types_members) */
    struct FreeList
    {
        struct FreeElem *m_head;       /* 0x00 — hkThreadMemory::FreeList::FreeElem */
        int m_numElem;                 /* 0x04 */
    };

    hkThreadMemory_vtbl *__vftable;    /* 0x000 */
    unsigned char _pad4[12];           /* 0x004 — db-verified gap */
    hkMemory *m_memory;                /* 0x010 */
    int m_referenceCount;              /* 0x014 */
    unsigned char _pad18[8];           /* 0x018 — db-verified gap */
    struct Stack m_stack;              /* 0x020 */
    int m_stackSize;                   /* 0x030 */
    int m_maxNumElemsOnFreeList;       /* 0x034 */
    struct FreeList m_free_list[17];   /* 0x038 */
    int m_row_to_size_lut[17];         /* 0x0C0 */
    char m_small_size_to_row_lut[35];  /* 0x104 */
    unsigned char _pad127[1];          /* 0x127 — db-verified gap */
    int m_large_size_to_row_lut[8];    /* 0x128 */
    unsigned char _pad148[8];          /* 0x148 — db-verified tail gap */
};                                     /* 0x150 */
typedef struct hkThreadMemory hkThreadMemory;

/* Memory-class tags. The database exposes these enumerator NAMES (used verbatim
   at the allocator call sites) but not their numeric values, so they are left
   as externs rather than invented. */
extern const int HK_MEMORY_CLASS_BASE;
extern const int HK_MEMORY_CLASS_BASE_CLASS;
extern const int HK_MEMORY_CLASS_DESTRUCTION;      /* DB types_enum_values = 0xC */
extern const int HK_MEMORY_CLASS_CDINFO;           /* DB types_enum_values = 0x28 */

void *hkThreadMemory_allocateChunk(hkThreadMemory *self, int size, int memoryClass);

/* Current thread's allocator. DEVIATION: at the call sites this is the raw TLS
   deref *(hkThreadMemory**)(*(int*)r13 + 704) (r13 = Xbox 360 thread pointer),
   wrapped here as an accessor. */
hkThreadMemory *hkThreadMemory_getCurrent(void);

extern const int HK_MEMORY_CLASS_ARRAY;
void hkThreadMemory_deallocateChunk(hkThreadMemory *self, void *chunk, int size, int memoryClass);

/* LIFO scratch allocator. DEVIATION: at the call sites these are the inlined
   bump/pop of the per-thread stack (top = *(self+32), limit = *(self+44),
   base marker = *(self+40); the slow path / empty-notify go through the
   allocator vtable). Wrapped here as the two public API calls. */
void *hkThreadMemory_allocateStack(hkThreadMemory *self, int numBytes); /* hkThreadMemory::allocateStack */
void hkThreadMemory_deallocateStack(hkThreadMemory *self, void *p);     /* hkThreadMemory::deallocateStack */
