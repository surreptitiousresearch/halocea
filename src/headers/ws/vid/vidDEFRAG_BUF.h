#pragma once
// ws-engine vid: generic (platform-agnostic) defrag buffer — extends apDEFRAG_BUF with the pair
// of D3D hardware buffer headers (index + vertex) that alias the backing memory range.
// DB-verified layout (types_members vidDEFRAG_BUF): base apDEFRAG_BUF@0 (size 96),
// pIBuf@0x60 (D3DIndexBuffer*), pVBuf@0x64 (D3DVertexBuffer*) — size 104.
//
// Boundary — this is the shared vid-layer base used by both the X360 and other-platform defrag
// buffer classes; its own methods (RemoveChunk/FitIntoFreeBlock/MoveChunk/DefragAllInside/dtor,
// CollectMemAreas, ReserveHoles, ShiftUp, AddHole, etc. — see funcs named `*@vidDEFRAG_BUF@@*`)
// are a separate, larger reversing task and are not implemented here. Only the layout is modeled
// so that vidDEFRAG_BUF_HCEX_X360 (the class actually reversed in this session) has a correctly
// DB-verified base.
#include "../ap/apDEFRAG_BUF.h"
#include "../../d3d_boundary.h" // D3DIndexBuffer, D3DVertexBuffer, D3DResource (opaque D3D boundary types)

struct vidDEFRAG_BUF; // fwd

// vtable — DB-verified (types_members vidDEFRAG_BUF_vtbl).
typedef struct vidDEFRAG_BUF_vtbl {
    void  (*RemoveChunk)(vidDEFRAG_BUF *self, apDEFRAG_CHUNK *chunk);
    char *(*FitIntoFreeBlock)(vidDEFRAG_BUF *self, apDEFRAG_CHUNK *chunk, int align);
    bool  (*MoveChunk)(vidDEFRAG_BUF *self, int block, apDEFRAG_CHUNK *chunk);
    void  (*DefragAllInside)(vidDEFRAG_BUF *self);
    void  (*dtr_vidDEFRAG_BUF)(vidDEFRAG_BUF *self);
} vidDEFRAG_BUF_vtbl;

struct vidDEFRAG_BUF : apDEFRAG_BUF {
    static vidDEFRAG_BUF_vtbl vftable; // boundary — not decompiled here

    D3DIndexBuffer  *pIBuf; // 0x60 (96) index-buffer hardware header aliasing this buffer's memory
    D3DVertexBuffer *pVBuf; // 0x64 (100) vertex-buffer hardware header aliasing this buffer's memory
};
