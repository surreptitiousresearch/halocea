#pragma once
// vidDEFRAG_CHUNK_TEX — ws-engine defrag-managed GPU buffer chunk backed by a texture surface.
// DB-verified layout (types_members vidDEFRAG_CHUNK_TEX): base vidDEFRAG_CHUNK@0 (size 32),
// pTex (txmD3D_TEX *) @32 — size 36. Note: its +0x20 slot aliases the same offset that
// vidDEFRAG_CHUNK_OBJ uses for pObj.
#include "vidDEFRAG_CHUNK.h"

struct txmD3D_TEX; // boundary — D3D texture wrapper

struct vidDEFRAG_CHUNK_TEX : vidDEFRAG_CHUNK {
    txmD3D_TEX *pTex; // 0x20 owning texture surface
};
