#pragma once
#include "../ds/WEAK_PTR.h"
// ws-engine ent: the "holder" sub-block of an entTRACKER — the resolved entity/instance/object a
// tracker currently points at. DB-verified layout (types_members trkHOLDER) — size 16 (0x10).

struct entENTITY; // entENTITY.h — game entity                boundary (weak-ref target)
struct animINST;  // anim subsystem — animated instance       boundary (ptr)
struct objOBJ;    // obj subsystem — game object              boundary (ptr)

struct trkHOLDER {
    bool                    isEntValid; // 0x00
    ds::WEAK_PTR<entENTITY> pEnt;       // 0x04 weak ref to the tracked entity
    animINST               *pInst;      // 0x08
    objOBJ                 *pObj;       // 0x0C

    // 0x82A49888 (?GetEnt@trkHOLDER@@QBAPAVentENTITY@@XZ) — the tracked entity (null when invalid).
    // boundary — body external to this batch.
    entENTITY *GetEnt() const;

    // (?IsValid@trkHOLDER@@...) — true when the holder currently resolves to a live target. Called
    // in the static form `trkHOLDER::IsValid(&h)` by aiBRAIN::TryToUseAnimDeathSmt.  boundary.
    static bool IsValid(const trkHOLDER *h);
};
