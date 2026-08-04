#pragma once
#include "mdlINFO_BASE_DESC.h"
#include "../ap/apSTATE_T.h" // Get/SetCdtAbilities carry an apSTATE_T<unsigned char>
// ws-engine mdl: base per-body model info record. DB-verified layout (types_members mdlINFO_BASE):
//   __vftable@0, pInst@4, desc@8 — size 0xC.

struct animINST; // anim — animated instance (fwd, boundary; ptr)
struct cdtREFINE;// cdt/cdtREFINE.h — collision refine state (GetSphereRefine return; ptr)

struct mdlINFO_BASE {
    void              *__vftable; // 0x00 mdlINFO_BASE_vtbl*
    animINST          *pInst;     // 0x04
    mdlINFO_BASE_DESC *desc;      // 0x08

    // 0x83073490 (?GetSphereRefine@mdlINFO_BASE@@UBAPAVcdtREFINE@@XZ) — virtual, const: the body's
    // collision sphere-refine record (null when none). boundary — body external to this batch.
    cdtREFINE *GetSphereRefine() const;

    // 0x83074B08 (?GetCdtAbilities@mdlINFO_BASE@@UBA?AV?$apSTATE_T@E@@XZ) — virtual, const: the body's
    // collision-ability state byte (returned by value). boundary — body external to this batch.
    apSTATE_T<unsigned char> GetCdtAbilities() const;
    // 0x830734A8 (?SetCdtAbilities@mdlINFO_BASE@@UAAXABV?$apSTATE_T@E@@@Z) — virtual: install the
    // collision-ability state byte. boundary — body external to this batch.
    void SetCdtAbilities(const apSTATE_T<unsigned char> &abilities);
};
