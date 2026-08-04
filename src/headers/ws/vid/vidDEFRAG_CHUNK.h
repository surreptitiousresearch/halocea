#pragma once
// ws-engine vid: generic (platform-agnostic) defrag chunk — extends apDEFRAG_CHUNK with a
// `content` tag identifying what kind of GPU buffer content the chunk holds (vertex vs index).
// DB-verified layout (types_members vidDEFRAG_CHUNK): base apDEFRAG_CHUNK@0 (size 28),
// content@0x1C (vidBUF_CONTENT, 1 byte) — size 29, padded to 32.
//
// Boundary — this is the shared vid-layer base used by both the X360 and other-platform defrag
// chunk classes; its own methods (DbgInfo/LockRead/LockWrite/Unlock/Alignment/MemMove/Id/dtor —
// see funcs named `*@vidDEFRAG_CHUNK@@*`) are a separate, larger reversing task and are not
// implemented here. Only the layout is modeled so that vidDEFRAG_CHUNK_OBJ_HCEX_X360 (the class
// actually reversed in this session) has a correctly DB-verified base.
#include "../ap/apDEFRAG_CHUNK.h"

// vidBUF_CONTENT — buffer-content kind tag (DB types_enum_values vidBUF_CONTENT).
enum vidBUF_CONTENT : unsigned char
{
    VID_BC_TEX             = 0,
    VID_BC_VERT            = 1,
    VID_BC_INDEX           = 2,
    VID_BC_VERT_LOCAL_MEM  = 3,
    VID_BC_INDEX_LOCAL_MEM = 4,
    VID_BC_TEX_MAIN_MEM    = 5,
    _VID_BC_MAX            = 6,
};

struct vidDEFRAG_CHUNK; // fwd

// vtable — DB-verified (types_members vidDEFRAG_CHUNK_vtbl). Same shape as apDEFRAG_CHUNK_vtbl,
// overridden per-instance at this level.
typedef struct vidDEFRAG_CHUNK_vtbl {
    const char *(__fastcall *DbgInfo)(vidDEFRAG_CHUNK *self);
    void       *(__fastcall *LockRead)(vidDEFRAG_CHUNK *self);
    void       *(__fastcall *LockWrite)(vidDEFRAG_CHUNK *self);
    void        (__fastcall *Unlock)(vidDEFRAG_CHUNK *self);
    int         (__fastcall *Alignment)(vidDEFRAG_CHUNK *self);
    int         (__fastcall *MemMove)(vidDEFRAG_CHUNK *self, void *dst, void *src, int size, bool overlap);
    void        (__fastcall *dtr_vidDEFRAG_CHUNK)(vidDEFRAG_CHUNK *self); // scalar-deleting dtor
    const char *(__fastcall *Id)(vidDEFRAG_CHUNK *self);
} vidDEFRAG_CHUNK_vtbl;

struct vidDEFRAG_CHUNK : apDEFRAG_CHUNK {
    static vidDEFRAG_CHUNK_vtbl vftable; // boundary — not decompiled here

    vidBUF_CONTENT content; // 0x1C buffer-content tag

    // boundary — not decompiled here. Constructs the apDEFRAG_CHUNK base only; leaves `content`
    // uninitialized (matches vidDEFRAG_CHUNK_OBJ_HCEX_X360's ctor, which never touches it either).
    vidDEFRAG_CHUNK();
};
