#pragma once
// ws-engine ap: static allocator interface — thin type-safe faade over the dlmalloc
// heap. All methods are static (mangle SA...); the class carries no instance state.
// Malloc honours the global default alignment (def_align) when it is 16.

// dlmalloc heap primitives (CRT-level boundary — not decompiled here).
extern "C" void  *dlmalloc(unsigned int size);
extern "C" void  *dlmemalign(unsigned int align, unsigned int size);
extern "C" void  *dlrealloc(void *ptr, unsigned int size);
extern "C" void  *dlrealloc_align(void *ptr, unsigned int align, unsigned int size);
extern "C" void   dlfree(void *ptr);

// Global default allocation alignment; when 16 every Malloc is 16-byte aligned.
extern int def_align;

// Atomic read of an int slot (Saber osync primitive) — boundary.
extern "C" int osLockedCompareAndSwap(int *slot, int compare, int exchange);

typedef struct apDLALLOC_IFACE {
    static void *Malloc(unsigned int size);
    static void  Free(void *ptr);
    static void *Realloc(void *ptr, unsigned int size);
    static void *Memalign(unsigned int size, unsigned int align);
    static void *ReallocAllign(void *ptr, unsigned int size, unsigned int align);
    static void  Freealign(void *ptr);
    // 0x82A20730 — zero-filling allocator (1 element of `size` bytes).
    static void *Calloc(unsigned int size);
} apDLALLOC_IFACE;
