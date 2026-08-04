#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/WEAK_PTR.h"
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dV.h"
// ws-engine gs: a geometry anchor — a resolved collision/hit reference (the entity, instance, bone
// and world point a ray or query landed on). DB-verified layout (types_members gsGEOM_ANCHOR) —
// size 60 (0x3C): instNameSafe@0, tplNameSafe@4, classNameSafe@8, state@12, pEnt@16, pInst@20,
// pObj@24, vClosest@28, distMin@40, ___u9@44 (anonymous 4-byte DB union), idFaceExt@48, syncId@52,
// objId@56.

struct cdtINFO;
struct entENTITY;
struct animINST; // anim — skeletal-animation instance (fwd, boundary pointer)
struct objOBJ;   // obj — game object (fwd, boundary pointer)

struct gsGEOM_ANCHOR {
    dsTSTRING<char>          instNameSafe;  // 0x00
    dsTSTRING<char>          tplNameSafe;   // 0x04
    dsTSTRING<char>          classNameSafe; // 0x08
    apSTATE_T<unsigned long> state;         // 0x0C
    ds::WEAK_PTR<entENTITY>  pEnt;          // 0x10
    animINST                *pInst;         // 0x14
    objOBJ                  *pObj;          // 0x18
    m3dV                     vClosest;      // 0x1C
    float                    distMin;       // 0x28
    unsigned int             ___u9;         // 0x2C DB anonymous 4-byte union slot (arms unresolved)
    int                      idFaceExt;     // 0x30
    int                      syncId;        // 0x34
    short                    objId;         // 0x38
    // 2 bytes padding to 0x3C

    // 0x-DB (??0gsGEOM_ANCHOR@@QAA@XZ) — default ctor.  boundary.
    gsGEOM_ANCHOR();
    // (??0gsGEOM_ANCHOR@@QAA@ABV0@@Z) — copy ctor.  boundary.
    gsGEOM_ANCHOR(const gsGEOM_ANCHOR &rhs);
    // ?gsGEOM_ANCHOR@@... construct from a collision-query result.  boundary.
    gsGEOM_ANCHOR(const cdtINFO &info);
    // ?GetEnt@gsGEOM_ANCHOR@@QBAPAVentENTITY@@XZ — the entity this anchor references (null if none).
    entENTITY *GetEnt() const;
    ~gsGEOM_ANCHOR();
};
