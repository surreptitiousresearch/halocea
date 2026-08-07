#pragma once
// vidDEFRAG_CHUNK_OBJ_HCEX_X360 — ws-engine defrag-managed GPU buffer chunk (Xbox 360), the HCEX
// static vertex/index buffer object. DB-verified layout (types_members
// vidDEFRAG_CHUNK_OBJ_HCEX_X360): base vidDEFRAG_CHUNK@0 (size 32) — no own fields added, size 32.
// It overrides vidDEFRAG_CHUNK's vtable (see vidDEFRAG_CHUNK_OBJ_HCEX_X360_vtbl below) but adds
// no storage of its own.
//
// This is the full C++ class header (methods + real inheritance) for the .cpp translation units
// that implement this class's methods. The legacy .c consumers (hcex_stat_ibuf_create.c,
// hcex_stat_vbuf_create.c, hcex_stat_vbuf_create_semi_dyn.c) instead include the C-compatible
// flat stub at src/headers/hcex/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h (same DB layout, POD-only) — this
// mirrors the existing dual-header split between src/headers/apDEFRAG_CHUNK.h (flat, C) and
// src/headers/ws/ap/apDEFRAG_CHUNK.h (full, C++).
#include "vidDEFRAG_CHUNK.h"

struct vidDEFRAG_CHUNK_OBJ_HCEX_X360; // fwd

// vtable — DB-verified (types_members vidDEFRAG_CHUNK_OBJ_HCEX_X360_vtbl). Same shape as
// vidDEFRAG_CHUNK_vtbl, overridden for this leaf class.
typedef struct vidDEFRAG_CHUNK_OBJ_HCEX_X360_vtbl {
    const char *(*DbgInfo)(vidDEFRAG_CHUNK_OBJ_HCEX_X360 *self);
    void       *(*LockRead)(vidDEFRAG_CHUNK_OBJ_HCEX_X360 *self);
    void       *(*LockWrite)(vidDEFRAG_CHUNK_OBJ_HCEX_X360 *self);
    void        (*Unlock)(vidDEFRAG_CHUNK_OBJ_HCEX_X360 *self);
    int         (*Alignment)(vidDEFRAG_CHUNK_OBJ_HCEX_X360 *self);
    int         (*MemMove)(vidDEFRAG_CHUNK_OBJ_HCEX_X360 *self, void *dst, void *src, int size, bool overlap);
    void        (*dtr_vidDEFRAG_CHUNK_OBJ_HCEX_X360)(vidDEFRAG_CHUNK_OBJ_HCEX_X360 *self, int deleteFlag); // scalar-deleting dtor — deleting dtor: vftable+0x18 holds ??_GvidDEFRAG_CHUNK_OBJ_HCEX_X360@@UAAPAXI@Z
    const char *(*Id)(vidDEFRAG_CHUNK_OBJ_HCEX_X360 *self);
} vidDEFRAG_CHUNK_OBJ_HCEX_X360_vtbl;

struct vidDEFRAG_CHUNK_OBJ_HCEX_X360 : vidDEFRAG_CHUNK {
    static vidDEFRAG_CHUNK_OBJ_HCEX_X360_vtbl vftable; // `vidDEFRAG_CHUNK_OBJ_HCEX_X360::`vftable''

    // 0x823F12DC — zero every base field (pDesc/pBuf/ptr/size/ptrPrev/state) and install this
    // class's vtable. `content` is left uninitialized, matching the base apDEFRAG_CHUNK ctor.
    vidDEFRAG_CHUNK_OBJ_HCEX_X360();

    // 0x823F1158 — reset the vtable pointer back to apDEFRAG_CHUNK's (no owned storage to free
    // at this level or at vidDEFRAG_CHUNK's; both destructor bodies are trivial and were fully
    // inlined by the compiler into this one function).
    ~vidDEFRAG_CHUNK_OBJ_HCEX_X360();

    // 0x823F131C — compiler-generated deleting-destructor entry point.
    vidDEFRAG_CHUNK_OBJ_HCEX_X360 *scalarDeletingDtor(unsigned char deleteFlags);

    // 0x823F16EC — memberwise copy of every base field plus `content`.
    vidDEFRAG_CHUNK_OBJ_HCEX_X360 &operator=(const vidDEFRAG_CHUNK_OBJ_HCEX_X360 &that);
};
