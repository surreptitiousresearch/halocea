#pragma once
#include "dsNODE_CACHE.h"
// ws-engine ds: the allocator policy ds::MAP uses to source/recycle its hash-bucket NODE records.
// DB-verified: dsNODE_CACHE_ALLOCATOR<...DUMMY> is the SAME 44-byte layout as dsNODE_CACHE<...DUMMY>
// (confirmed by MAP's ctor constructing a dsNODE_CACHE<DUMMY> directly at the `allocator` member's
// offset, and MAP's dtor tearing down `allocator.nodeList`/`Term` via the dsNODE_CACHE API) — it
// publicly derives from dsNODE_CACHE<NODE> and simply renames the alloc/free entry points to the
// Alloc/Delete vocabulary ds::MAP's private helpers expect.

template<class NODE>
struct dsNODE_CACHE_ALLOCATOR : dsNODE_CACHE<NODE> {
    // 0x8255BB58 — forwards to the dsNODE_CACHE(apCL*) base ctor.
    dsNODE_CACHE_ALLOCATOR(apCL cl);
    // 0x8255C2A0 — Term() + destroy nodeList (mirrors dsNODE_CACHE's own dtor; the compiler
    // inlines this member teardown directly at ds::MAP's dtor instead of calling this dtor there).
    ~dsNODE_CACHE_ALLOCATOR();

    // 0x8255C6A8 — alias for dsNODE_CACHE::GetEmpty.
    NODE *Alloc();
    // 0x8255A220 — alias for dsNODE_CACHE::Release.
    void Delete(NODE *pNode);
};
