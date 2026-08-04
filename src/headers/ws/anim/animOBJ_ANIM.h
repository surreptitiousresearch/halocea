#pragma once
// ws-engine anim: per-object resolved animation channel data (the object/sequence pairing returned
// by animMNG::GetObjAnim). Only the accessors used by animExtractObjAnim are declared here; the
// full record layout is a boundary (not reversed in this batch).
#include "../m3d/m3dQUAT.h"

struct animTRS;

typedef struct animOBJ_ANIM {
    // 0x82... (?GetRotation@animOBJ_ANIM@@QBAXMHPAVm3dQUAT@@@Z) — sample the rotation channel at
    // `frame`; `additive` selects the additive/blend variant (object state bit 0x20000). Result
    // written to *out. boundary.
    void GetRotation(float frame, int additive, m3dQUAT *out) const;
} animOBJ_ANIM;

// 0x82... (?animExtractAnimTrs@@YAXPBUanimOBJ_ANIM@@MHPAVanimTRS@@@Z) — sample the full decomposed
// transform (trans/scale/rot + flags) of `objAnim` at `frame` into *trs. `additive` as above.
// boundary (free function, external to this batch).
void animExtractAnimTrs(const animOBJ_ANIM *objAnim, float frame, int additive, animTRS *trs);
