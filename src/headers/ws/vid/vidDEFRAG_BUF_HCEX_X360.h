#pragma once
// vidDEFRAG_BUF_HCEX_X360 — ws-engine defrag-managed GPU backing buffer (Xbox 360), the HCEX
// static vertex/index buffer allocation. DB-verified layout (types_members
// vidDEFRAG_BUF_HCEX_X360): base vidDEFRAG_BUF@0 (size 104) — no own fields added, size 104.
// Its own methods override the base vtable slots that differ per-buffer-kind; the rest
// (FitIntoFreeBlock, MoveChunk) are inherited unmodified from vidDEFRAG_BUF/apDEFRAG_BUF.
#include "vidDEFRAG_BUF.h"

struct vidDEFRAG_BUF_HCEX_X360; // fwd

// vtable — DB-verified (types_members vidDEFRAG_BUF_HCEX_X360_vtbl). Same shape as
// vidDEFRAG_BUF_vtbl, overridden for this leaf class.
typedef struct vidDEFRAG_BUF_HCEX_X360_vtbl {
    void  (*RemoveChunk)(vidDEFRAG_BUF_HCEX_X360 *self, apDEFRAG_CHUNK *chunk);
    char *(*FitIntoFreeBlock)(vidDEFRAG_BUF_HCEX_X360 *self, apDEFRAG_CHUNK *chunk, int align);
    bool  (*MoveChunk)(vidDEFRAG_BUF_HCEX_X360 *self, int block, apDEFRAG_CHUNK *chunk);
    void  (*DefragAllInside)(vidDEFRAG_BUF_HCEX_X360 *self);
    void  (*dtr_vidDEFRAG_BUF_HCEX_X360)(vidDEFRAG_BUF_HCEX_X360 *self);
} vidDEFRAG_BUF_HCEX_X360_vtbl;

struct vidDEFRAG_BUF_HCEX_X360 : vidDEFRAG_BUF {
    static vidDEFRAG_BUF_HCEX_X360_vtbl vftable; // `vidDEFRAG_BUF_HCEX_X360::`vftable''

    // 0x823F164C — construct the apDEFRAG_BUF base, null pIBuf/pVBuf, install this class's vtable.
    vidDEFRAG_BUF_HCEX_X360();

    // 0x823F16AC — reset the vtable pointer to apDEFRAG_BUF's and free the four dsVECTOR backing
    // arrays (chunks/freeBlocks/chunksRT/usedBlocks). pIBuf/pVBuf are NOT freed here (they are
    // owned by CreateBuf's allocation, not by this dtor — reproduced as-is from the DB).
    ~vidDEFRAG_BUF_HCEX_X360();

    // 0x823F1914 — compiler-generated deleting-destructor entry point.
    vidDEFRAG_BUF_HCEX_X360 *scalarDeletingDtor(unsigned char deleteFlags);

    // 0x823F1688 — no-op override (this buffer kind never needs in-place defragmentation).
    void DefragAllInside();

    // 0x823F1680 — thunk straight through to apDEFRAG_BUF::RemoveChunk.
    void RemoveChunk(apDEFRAG_CHUNK *pChunk);
};
