#pragma once
#include "../ds/dsVECTOR.h"
#include "aiFEE.h"
#include "aiWAH_SYS.h" // aiWAH_SYS::IsIncludeFEECheck

// ws-engine ai08 (a8_plan_fe.cpp) — the "enemy filter": a working set of aiFEE records (foe/entity
// evaluations) that is progressively narrowed by a series of Pass* predicates and sorted by
// distance. DB-verified layout (types_members aiFILT_ENEMY) — size 20 (0x14): a single
// dsVECTOR<aiFEE,8>.

struct aiBRAIN;    // ai08 — per-unit AI brain            boundary (fwd)
struct entENTITY;  // ent subsystem — game entity          boundary (fwd)
struct aiWATCHER;  // aiWATCHER.h
typedef struct m3dV m3dV;

// Empty comparison functor: orders two aiFEE records by their precomputed dist4cmp weight.
// DB: aiCMP_EDPAIR carries no data members (stateless functor passed by value/const-ref).
struct aiCMP_EDPAIR {
    // 0x8321F6D8 — strcmp-style: -1 if a<b, 0 if equal, 1 if a>b (by dist4cmp).
    int cmp(const aiFEE *a, const aiFEE *b) const;
};

struct aiFILT_ENEMY {
    dsVECTOR<aiFEE, 8> arr; // 0x00 working set of foe/entity evaluations

    // Interface an external caller supplies to Pass(): Examine() is invoked on every element
    // (observation pass), then Apply() decides per element whether it survives the filter.
    // DB layout: aiFILT_ENEMY::PASS_IFACE = { __vftable } (size 4).
    struct PASS_IFACE {
        void *__vftable;                    // 0x00 PASS_IFACE_vtbl*
        void Examine(const aiFEE *fee);     // vtbl+0x4 — observe an element
        bool Apply(const aiFEE *fee);       // vtbl+0x8 — keep this element?
    };

    // 0x8321FB08 — clear the set and (re)fill it with a FEE for every watcher the planner's
    // aiWAH_SYS holds, using the "Regular" include predicate bound to `brain`.
    void Start(const aiBRAIN *brain);

    // 0x83220488 — drop every FEE whose watcher `brain`'s mind does not accept as an enemy.
    void PassAllyOpp(aiBRAIN *brain);
    // 0x83220560 — if any FEE's entity is shootable (entENTITY.stateEnt bit 10), drop all the
    // non-shootable FEEs; otherwise leave the set untouched.
    void PassShootable(aiBRAIN *brain);
    // 0x83220718 — drop every FEE whose watcher will not accept `brain` as an enemy.
    void PassEnemiesCount(aiBRAIN *brain);

    // 0x83220800 — recompute dist/dist4cmp on every FEE (melee vs ranged metric selected from the
    // brain's descriptor).
    void FillDistance(const aiBRAIN *brain);
    // 0x83220878 — FillDistance, then sort the set ascending by dist4cmp.
    void PassDistance(const aiBRAIN *brain);
    // 0x832208C8 — drop every FEE not potentially visible from `brain`'s eye.
    void PassPotentiallyVisible(aiBRAIN *brain);

    // 0x832209F0 — run `pass`: Examine every element, then Apply()-filter the set.
    void Pass(PASS_IFACE *pass);
    // 0x83220B50 — the first surviving FEE's entity, or null if the set is empty.
    entENTITY *GetAny();

    // 0x83220BB0 (?GetFirst@aiFILT_ENEMY@@QAAPAVentENTITY@@XZ) — entity of the first candidate
    // (null when empty). REVERSED: aiFILT_ENEMY__GetFirst.cpp.
    entENTITY *GetFirst();
    // 0x83220C10 (?GetClosest@aiFILT_ENEMY@@QAAPAVentENTITY@@XZ) — entity of the candidate with the
    // smallest dist4cmp weight (null when empty). REVERSED: aiFILT_ENEMY__GetClosest.cpp.
    entENTITY *GetClosest();
    // 0x83220D68 (?HasAChoice@aiFILT_ENEMY@@QAA_NAAPAVentENTITY@@@Z) — with 0 or 1 candidates,
    // write that entity (or null) into `out` and return false (nothing to choose); with >1
    // candidates return true (caller must run visibility selection). REVERSED: aiFILT_ENEMY__HasAChoice.cpp.
    bool HasAChoice(entENTITY *&out);

    // The result of SelectFirstVisible. DB-verified values (types_enum_values aiFILT_ENEMY::SELECT_RESULT).
    enum SELECT_RESULT {
        FE_RES_WAIT = 0, // results not ready — a request was (re)posted; retry next frame
        FE_RES_DONE = 1, // results are in; `out` holds the selected entity (or null)
    };

    // 0x83221120 (?SelectFirstVisible@aiFILT_ENEMY@@QAA?AW4SELECT_RESULT@1@PAVaiBRAIN@@AAPAVentENTITY@@@Z)
    // — if find-enemy results are ready for `viewer`, write the visible candidate (or null) into
    // `out` and return FE_RES_DONE; otherwise (re)post the multi-candidate request and return
    // FE_RES_WAIT. REVERSED: aiFILT_ENEMY__SelectFirstVisible.cpp.
    SELECT_RESULT SelectFirstVisible(aiBRAIN *viewer, entENTITY *&out);

    // 0x8321FB00 (?Clear@aiFILT_ENEMY@@QAAXXZ) — clear the enemy-filter candidate list. boundary.
    void Clear();

private:
    // 0x83220F50 (?PostMultipleRequest@aiFILT_ENEMY@@AAAXPAVaiBRAIN@@@Z) — build and post a
    // find-enemy visibility request to the planner's default CDT system covering every current
    // candidate (capped at 10). REVERSED: aiFILT_ENEMY__PostMultipleRequest.cpp.
    void PostMultipleRequest(aiBRAIN *viewer);

    // ai08 private helper feeding PassPotentiallyVisible: derive the eye/target sample points for
    // the visibility check of `watcher` from `brain`. Body external to this batch.  boundary.
    void GetVisCheckParams(aiBRAIN *brain, aiWATCHER *watcher, m3dV *outEye, m3dV *outTarget);
};

// ai08-internal ('anonymous namespace') include predicate used by Start(): an
// aiWAH_SYS::IsIncludeFEECheck carrying the querying brain. Layout: { __vftable@0, pBrain@4 }.
struct aiFILT_ENEMY_IsIncludeFEECheck_Regular : aiWAH_SYS::IsIncludeFEECheck {
    aiBRAIN *pBrain; // 0x04
};

// 'anonymous namespace'::IsIncludeFEECheck_Regular::`vftable` — the predicate's vtable object.
// External definition (ai08 TU-local); referenced only to stamp the checker's __vftable.  boundary.
extern void *aiFILT_ENEMY_IsIncludeFEECheck_Regular_vftable;
