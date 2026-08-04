#include <stdint.h>

#pragma once
// ws-engine obj: external boundary declarations for the dl* memory allocator used by
// the obj subsystem. Not reversed here — declared so obj sources stay type-checkable.

extern "C" void *dlMemalign(uint32_t size, uint32_t align, const char *file, uint32_t line);
extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);
extern "C" void dlFree(void *ptr);
extern "C" void dlFreeAligned(void *ptr);
