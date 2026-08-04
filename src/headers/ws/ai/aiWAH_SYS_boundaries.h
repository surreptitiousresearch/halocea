#pragma once
// Boundary declarations for the aiWAH_SYS (a8_plan_wah.cpp) re-source batch: cross-subsystem
// singletons/free functions and the deep brain/status members these methods reach but that are
// reversed in their own subsystems. Only the touched surface is declared — not authoritative
// full layouts.

#include "aiSTATUS.h"    // canonical aiSTATUS (peekerSelf / dbgDuplicatePeekerSelf)

struct iaIACTOR;         // ia subsystem — interactive actor            boundary (fwd)
struct entENTITY;        // ent subsystem — game entity                 boundary (fwd)
struct aiWATCHER_BRAIN;  // aiWATCHER_BRAIN.h
struct aiBRAIN;          // ai08 — per-unit AI brain                     boundary (fwd)

// Debug RTTI predicate: true when `brain`'s dynamic type derives from aiBRAIN. The binary inlines
//   dsTYPE_ID::IsDerivedFrom(brain->TypeID(), &aiBRAIN_DESC::TYPE_ID)
// — modelled as a boundary helper (the type-id machinery is reversed elsewhere). boundary.
bool aiBrainDynTypeIsBrain(const aiBRAIN *brain);

// gs player access (gs subsystem). boundary. The returned player entity is used purely through
// its iaIACTOR base (Demand/Forgo), which sits at offset 0 in the binary — typed as iaIACTOR*.
int gsGetPlayerCount();
iaIACTOR *gsGetPlayer(int index);

// ap logging (variadic ~AI~ channel). boundary.
extern void _apLog(const char *fmt, ...);

// aiSTATUS (aiBRAIN.st) is defined once in aiSTATUS.h (included above); the peeker back-pointers
// peekerSelf@0x08 / dbgDuplicatePeekerSelf@0x14 the watcher system writes are typed there.

// Strong-assert plumbing (boundary) — mirrors the cseries STRONG_ASSERT macro.
// STRONG_ASSERT_DUMMY has a single canonical home (dbg/STRONG_ASSERT_DUMMY.h); include it rather
// than redefining the struct here (avoids C2011 when a TU also pulls the canonical via aiBRAIN.h).
#include "../dbg/STRONG_ASSERT_DUMMY.h"
extern int IGNORE_STRONG_ASSERT;              // _IGNORE_STRONG_ASSERT
extern unsigned char STRONG_ASSERT_INFO_STUB; // empty_string (assert "info" byte)
