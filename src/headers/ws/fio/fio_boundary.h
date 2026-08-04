#pragma once
#include "fioFILE_DISK.h"
// Boundary declarations for helpers called by the fio stream methods but not decompiled
// in this batch: the OS file layer, the debug allocator (dl*), byte-reorder, logging
// and the strong-assert stub.

struct osFILE_HANDLE_DUMMY; // boundary — opaque OS file handle
struct osHANDLE_DUMMY;      // boundary — opaque OS wait/signal handle (see osASYNC_INFO.h)

// --- OS file layer (os*) — thin CRT/Win32 wrappers, not decompiled ---
osFILE_HANDLE_DUMMY *osFileOpen(const char *name, int mode); // boundary — real 2nd arg is dsFLAGS<OS_FILE,int> by value (4-byte)
void          osFileClose(osFILE_HANDLE_DUMMY *fp);          // boundary
int           osFileWrite(osFILE_HANDLE_DUMMY *fp, const void *buf, int size); // boundary — returns bytes written
int           osFileRead(osFILE_HANDLE_DUMMY *fp, void *buf, int size);       // boundary — returns bytes read
unsigned int  osFileGetSize(osFILE_HANDLE_DUMMY *fp);        // boundary
void          osDestroySignal(osHANDLE_DUMMY *signal);       // boundary — releases an OS wait/signal handle

// --- debug heap allocator (dl*) ---
extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);                       // boundary
void *dlReallocAligned(void *p, unsigned int size, unsigned int align, const char *file, unsigned int line); // boundary
extern void dlFree(void *ptr);        // boundary
extern void dlFreeAligned(void *ptr); // boundary

namespace ds {
// Endian byte-swap `data` in place: `count` bytes grouped into `gain`-byte elements. boundary.
void ByteReorder(void *data, int count, int gain);
} // namespace ds

// Formatted engine log (varargs). boundary.
void _apLog(const char *fmt, ...);

// Strong-assert stub — Crash() aborts; guarded by the IGNORE_STRONG_ASSERT global.
extern int IGNORE_STRONG_ASSERT;          // boundary — global assert-suppression flag
extern unsigned char g_assertContextByte; // boundary — assert message context byte @ 0x8200155A
#include "../dbg/STRONG_ASSERT_DUMMY.h" // canonical (member + static Crash overloads) — avoids C2011
