#pragma once
// ws-engine vid: base GPU-buffer memory manager. DB-verified layout (types_members vidMEM_MNG,
// size 52): vftable@0, buffers@4, curDynBuf@24. It owns the set of vidBUFFER backing stores and
// the per-content dynamic-buffer cursor array; vidVBUF_MNG derives from it. Only the layout is
// modeled here — its own methods are a separate reversing task (boundary).

#include "../ds/dsVECTOR.h"
#include "../ds/dsCONST_ARRAY.h"

struct vidMEM_MNG_vtbl; // boundary -- memory-manager vtable
struct vidBUFFER;       // boundary -- GPU buffer backing store
enum vidBUF_CONTENT : unsigned char; // full definition in vidDEFRAG_CHUNK.h

typedef struct vidMEM_MNG
{
    vidMEM_MNG_vtbl                *__vftable; /* 0x00 */
    dsVECTOR<vidBUFFER *, 8>        buffers;   /* 0x04 */
    dsCONST_ARRAY<vidBUFFER *, 6>   curDynBuf; /* 0x18 (24) */

    // 0x82D542B0 (?GetDynBuf@vidMEM_MNG@@QBAPAUvidBUFFER@@W4vidBUF_CONTENT@@@Z) -- return the
    // current dynamic buffer backing store for the given content kind. boundary (external to
    // this batch); used by d3dVBUF_MNG::DiscardDynAll.
    vidBUFFER *GetDynBuf(vidBUF_CONTENT cont) const;
} vidMEM_MNG; /* 52 bytes */
