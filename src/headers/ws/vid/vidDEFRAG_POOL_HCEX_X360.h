#pragma once
// vidDEFRAG_POOL_HCEX_X360 — ws-engine defrag pool for the HCEX X360 static vertex/index buffer
// arena (id "hcex_x360"). DB-verified layout (types_members vidDEFRAG_POOL_HCEX_X360): base
// apDEFRAG_POOL@0 (size 44) — no own fields added, size 44. Buffers in this pool are backed by
// physical console memory (vidAllocPhysMem) rather than the generic heap; FreeBuf/AllocBuf never
// actually release/allocate per-call (the whole 16 MB arena is allocated once and never returned).
#include "../ap/apDEFRAG_POOL.h"
#include "../ap/apDEFRAG_BUF.h"

struct vidDEFRAG_POOL_HCEX_X360; // fwd

// vtable — DB-verified (types_members vidDEFRAG_POOL_HCEX_X360_vtbl). Same shape as
// apDEFRAG_POOL_vtbl, overridden for this leaf class.
typedef struct vidDEFRAG_POOL_HCEX_X360_vtbl {
    void          (__fastcall *dtr_vidDEFRAG_POOL_HCEX_X360)(vidDEFRAG_POOL_HCEX_X360 *self);
    void         *(__fastcall *AllocBuf)(vidDEFRAG_POOL_HCEX_X360 *self, int size, int *outIdx);
    bool          (__fastcall *FreeBuf)(vidDEFRAG_POOL_HCEX_X360 *self, apDEFRAG_BUF *buf);
    const char   *(__fastcall *Id)(vidDEFRAG_POOL_HCEX_X360 *self);
    apDEFRAG_BUF *(__fastcall *CreateBuf)(vidDEFRAG_POOL_HCEX_X360 *self, char *base, int size);
    void          (__fastcall *FitChunk)(vidDEFRAG_POOL_HCEX_X360 *self, apDEFRAG_CHUNK *chunk);
    void          (__fastcall *DumpStat)(vidDEFRAG_POOL_HCEX_X360 *self);
} vidDEFRAG_POOL_HCEX_X360_vtbl;

struct vidDEFRAG_POOL_HCEX_X360 : apDEFRAG_POOL {
    static vidDEFRAG_POOL_HCEX_X360_vtbl vftable; // `vidDEFRAG_POOL_HCEX_X360::`vftable''

    // 0x823F1354 — zero the buffers/descs dsVECTORs (including their apCL debug-tag members) and
    // install this class's vtable.
    vidDEFRAG_POOL_HCEX_X360();

    // 0x823F1440 — thunk straight through to apDEFRAG_POOL::~apDEFRAG_POOL.
    ~vidDEFRAG_POOL_HCEX_X360();

    // 0x823F1494 — compiler-generated deleting-destructor entry point (vector-deleting variant;
    // DB shows only the (this, flags) shape here, no array-count parameter).
    vidDEFRAG_POOL_HCEX_X360 *vectorDeletingDtor(unsigned char deleteFlags);

    // 0x823F13D4 — request the whole 16 MB (0x1000000) physical-memory arena in one shot via
    // vidAllocPhysMem; STRONG_ASSERTs that `minSize` fits. `bufSize` is always written 0x1000000.
    void *AllocBuf(int minSize, int *bufSize);

    // 0x823F1830 — heap-allocate a vidDEFRAG_BUF_HCEX_X360 (via its already-reversed ctor), then
    // dlCalloc a pair of 0x20-byte D3D hardware buffer headers into pIBuf/pVBuf and point both at
    // `ptr` (XGSet*BufferHeader + XGOffsetResourceAddress), so the same physical memory range is
    // aliased as both an index buffer and a vertex buffer. Deviation: the decompiler shows this
    // fully inlined as raw `operator new` + `apDEFRAG_BUF::apDEFRAG_BUF` + manual vtable/field
    // patch (the `v6[1].__vftable`/`v6[1].ptr` slice is vidDEFRAG_BUF_HCEX_X360::pIBuf/pVBuf at
    // DB-verified offsets +0x60/+0x64) — reconstructed here as the equivalent
    // `new vidDEFRAG_BUF_HCEX_X360()` expression, since that ctor's body matches byte-for-byte.
    apDEFRAG_BUF *CreateBuf(char *ptr, int size);

    // 0x823F13B4 — always returns false: buffers in this pool are never actually freed.
    bool FreeBuf(apDEFRAG_BUF *buf);

    // 0x823F13A8 — instance Id, same string as the static Name().
    const char *Id() const;

    // 0x823EF6C8 — static pool identifier, "hcex_x360".
    static const char *Name();
};
