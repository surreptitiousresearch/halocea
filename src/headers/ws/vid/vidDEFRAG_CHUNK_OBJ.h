#pragma once
// vidDEFRAG_CHUNK_OBJ — ws-engine defrag-managed GPU buffer chunk that is backed by a render
// object. DB-verified layout (types_members vidDEFRAG_CHUNK_OBJ): base vidDEFRAG_CHUNK@0 (size 32),
// pObj (objOBJ *) @32 — size 36.
#include "vidDEFRAG_CHUNK.h"

struct objOBJ; // boundary — ws render object

struct vidDEFRAG_CHUNK_OBJ : vidDEFRAG_CHUNK {
    objOBJ *pObj; // 0x20 owning render object
};
