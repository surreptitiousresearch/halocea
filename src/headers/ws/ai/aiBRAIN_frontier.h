#pragma once
#include "../m3d/m3dV.h"

// Boundary accessors into aiBRAIN's internals for the a8_planner.cpp batch.
//
// aiBRAIN's full layout is the NEXT FRONTIER; only these DB-verified members are needed here
// (types_members aiBRAIN): vtable@0, body@448 (dsSHARED_PTR<aiBODY_IFACE>), mind@456 (aiMIND),
// names@748 (aiNAMES). The planner reaches an AI's nav-goal object and its debug name through
// sub-fields of mind/names whose own layouts are deferred; those offsets are reproduced verbatim
// from the disassembly (a8_planner.cpp) and documented at each use site, rather than fabricating
// aiMIND/aiNAMES bodies.

struct navSYS;

// The per-brain nav-goal object lives at mind+40 (== aiBRAIN+496). The planner drives two of its
// virtuals: vtable byte-offset +12 = SetNavSystem(navSYS*, bool), +16 = GetNavSystem(). The exact
// vtable-index mapping is a boundary, so these are dispatched via documented byte offsets to stay
// faithful to the disassembly.
static inline void *aiBRAIN_NavGoal(void *brain) {
    // brain->mind (@456) + 40 == aiBRAIN+496 == the nav-goal object pointer slot.
    return *reinterpret_cast<void **>(reinterpret_cast<char *>(brain) + 496);
}
static inline void aiNavGoal_SetNavSystem(void *goal, navSYS *ns, bool force) {
    typedef void (*Fn)(void *, navSYS *, int);
    Fn fn = *reinterpret_cast<Fn *>(*reinterpret_cast<char **>(goal) + 12);
    fn(goal, ns, force ? 1 : 0);
}
static inline navSYS *aiNavGoal_GetNavSystem(void *goal) {
    typedef navSYS *(*Fn)(void *);
    Fn fn = *reinterpret_cast<Fn *>(*reinterpret_cast<char **>(goal) + 16);
    return fn(goal);
}
static inline const char *aiBRAIN_DbgName(void *brain) {
    // brain->names (@748); its leading field is the unit's debug name (used only for %s logging).
    return *reinterpret_cast<const char **>(reinterpret_cast<char *>(brain) + 748);
}

struct entENTITY;
struct m3dV;
struct m3dOBB;

// Predicate the planner uses to decide whether a brain should be told about a flying object:
// it gates on a per-species-descriptor flag and on status.mind bit 20 (both deep aiMIND/aiSTATUS
// internals whose offsets are the next frontier). Hoisted to a boundary rather than fabricating
// those sub-offsets. Body reversed with aiMIND/aiSTATUS (a8_planner.cpp gate at 0x83164FF0).
extern bool aiBRAIN_ShouldNoticeFlyObject(void *brain);

// Debug-only: record the incoming fly-object OBB and its expanded trajectory OBB into
// brain->dbg.flyObj (aiBRAIN.dbg @804, aiDEBUG::flyObj). Debug instrumentation; boundary.
extern void aiBRAIN_DbgSetFlyObj(void *brain, const m3dOBB *obb, const m3dOBB *trajObb);

// aiMIND::NoticeFlyObject(&brain->mind, from, dir) — brain->mind is DB-verified @456. boundary.
extern void aiMIND_NoticeFlyObject(void *mind, const m3dV *from, const m3dV *dir);
static inline void *aiBRAIN_Mind(void *brain) {
    return reinterpret_cast<char *>(brain) + 456; // aiBRAIN.mind @456 (DB-verified)
}

// --- LocalizeToNavSystem gates (a8_planner.cpp 0x831678C0) ------------------------------------
// True when this brain should attempt nav-system (re)localization: combines the brain's
// virtual "localize blocked?" query (vtable+464), its species descriptor's allow/force flags
// (dscDESC[40].pBrand bytes), and whether the nav-goal currently has no nav-system. All three
// are deep aiMIND/dscDESC internals whose sub-layouts are the next frontier. boundary.
extern bool aiBRAIN_ShouldAttemptLocalize(void *brain);

// Brain world position via its GetPos virtual (vtable+448). boundary.
extern void aiBRAIN_GetPos(void *brain, m3dV *out);

// Scoped profiling marker mirroring apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME>; boundary (ap).
struct aiScopedTimer {
    aiScopedTimer(const char *label);
    ~aiScopedTimer();
};
