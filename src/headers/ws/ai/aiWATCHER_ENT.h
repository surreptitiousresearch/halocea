#pragma once
#include "aiWATCHER.h"
#include "../ds/WEAK_PTR.h"
#include "../prop/propGAME_INFO_TEAM.h"
// ws-engine ai08 — a watcher bound to a plain game entity (not a brain).
// DB-verified layout (types_members aiWATCHER_ENT) — size 0x60:
//   aiWATCHER@0 (76B base), entity@0x4C, teamIDcached@0x50, teamID@0x54, mayBeFoe@0x5C.

struct entENTITY; // entENTITY.h
struct iaIACTOR;  // ia subsystem — interactive actor   boundary (fwd)

struct aiWATCHER_ENT : aiWATCHER {
    ds::WEAK_PTR<entENTITY> entity;            // 0x4C weak ref to the entity
    bool                   teamIDcached;       // 0x50
    // 0x51..0x53 padding
    propGAME_INFO_TEAM     teamID;             // 0x54
    bool                   mayBeFoe;           // 0x5C

    // Virtuals (aiWATCHER_vtbl) reached by aiWAH_SYS. Bodies are ai08 boundary.
    bool Is(const iaIACTOR *actor);            // vtbl+0x4
    bool IsEntity();                           // vtbl+0x14

    // Static acceptance predicate (SA) — reversed in this batch.
    static bool IsAcceptActor(iaIACTOR *actor); // 0x8328EBA0

    // 0x8328FC88 (?IsDead@aiWATCHER_ENT@@UBA_NXZ) — virtual const: the tracked entity's fireable
    // component reports a dead/dying state. REVERSED: src/ws/ai/aiWATCHER_ENT__IsDead.cpp
    bool IsDead() const;

    // 0x8328DF18 / 0x8328DF98 (?GetMinHeightFlesh@ / ?GetHeight@aiWATCHER_ENT@@UBAMXZ) — virtual
    // const: forward the tracked game-info's min-flesh height / full height (0 when invalid or no
    // game info). REVERSED: src/ws/ai/aiWATCHER_ENT__GetMinHeightFlesh.cpp, aiWATCHER_ENT__GetHeight.cpp
    float GetMinHeightFlesh() const;
    float GetHeight() const;

    // Reversed in this batch (src/ws/ai/aiWATCHER_ENT__*.cpp).
    void GetBodyMatrNav(struct m3dMATR &matr) const;                       // 0x8328EE38 (virtual)
    void GetBodyCS(struct m3dV &pos, struct m3dV &dir, struct m3dV &dirUp) const; // 0x8328EF00
    struct m3dV GetReticle(struct m3dV &eye) const;                        // 0x8328EFF0 (virtual)
    unsigned int GetUID() const;                                           // 0x8328F0C0 (virtual)
};
