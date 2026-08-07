#pragma once
// ws-engine anim: per-object resolved animation channel data (the object/sequence pairing returned
// by animMNG::GetObjAnim). Four sampled channels, each an initial value plus an optional spline:
// translation, rotation, scale and visibility.
// DB-verified full layout (types_members animOBJ_ANIM): iniTranslation@0x00, pTranslation@0x0C,
// iniRotation@0x10, pRotation@0x20, iniScale@0x24, pScale@0x30, iniVisibility@0x34,
// pVisibility@0x38, state@0x3C — sizeof 64 (0x40) after the 3-byte tail pad.
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dQUAT.h"
#include "../m3d/m3dV.h"

struct animTRS;
struct m3dSPL; // ../m3d/m3dSPL.h — pointer only

typedef struct animOBJ_ANIM {
    m3dV                      iniTranslation; // 0x00 translation when pTranslation is null
    m3dSPL                   *pTranslation;   // 0x0C translation channel spline
    m3dQUAT                   iniRotation;    // 0x10
    m3dSPL                   *pRotation;      // 0x20 rotation channel spline
    m3dV                      iniScale;       // 0x24
    m3dSPL                   *pScale;         // 0x30 scale channel spline
    float                     iniVisibility;  // 0x34
    m3dSPL                   *pVisibility;    // 0x38 visibility channel spline
    apSTATE_T<unsigned char>  state;          // 0x3C

    // 0x82... (?GetRotation@animOBJ_ANIM@@QBAXMHPAVm3dQUAT@@@Z) — sample the rotation channel at
    // `frame`; `additive` selects the additive/blend variant (object state bit 0x20000). Result
    // written to *out. boundary.
    void GetRotation(float frame, int additive, m3dQUAT *out) const;
} animOBJ_ANIM;

// 0x82... (?animExtractAnimTrs@@YAXPBUanimOBJ_ANIM@@MHPAVanimTRS@@@Z) — sample the full decomposed
// transform (trans/scale/rot + flags) of `objAnim` at `frame` into *trs. `additive` as above.
// boundary (free function, external to this batch).
void animExtractAnimTrs(const animOBJ_ANIM *objAnim, float frame, int additive, animTRS *trs);
