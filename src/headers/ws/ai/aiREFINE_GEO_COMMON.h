#pragma once
#include "../cdt/cdtREFINE.h"
// ws-engine ai08 — shared base of the geometric-refine filters. DB-verified (types_members
// aiREFINE_GEO_COMMON, size 32): a cdtREFINE base with no additional members (own vtable only).

struct aiBRAIN;

struct aiREFINE_GEO_COMMON : cdtREFINE {
    // ??0aiREFINE_GEO_COMMON@@QAA@PAVanimINST@@K@Z — boundary.
    aiREFINE_GEO_COMMON(animINST *pInstSelf, unsigned long flags);
    // ??1aiREFINE_GEO_COMMON@@UAA@XZ — boundary.
    ~aiREFINE_GEO_COMMON();
    // ?IsInclInst@aiREFINE_GEO_COMMON@@UAAHPAVanimINST@@@Z — boundary (virtual via cdtREFINE vtbl).
    int IsInclInst(animINST *inst);
};
