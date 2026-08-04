#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai: a named speed/gait preset (moved verbatim from aiGOAL_BASE.h so value consumers
// like bhpPATH_PAR can embed it without the full goal-module header).

struct psSECTION;

// aiSPD — a single speed/gait descriptor. DB-verified layout (types_members aiSPD), size 52.
struct aiSPD {
    dsSTRID name;                 // 0x00 speed name (interned)
    float   maxDist;              // 0x04
    float   maxDistDefault;       // 0x08
    float   tempMaxDist;          // 0x0C
    float   minDistToEnemy;       // 0x10
    float   maxDistToGoal;        // 0x14
    bool    isHOJumpSlideEnabled; // 0x18
    float   hoJumpSlideDist;      // 0x1C
    bool    isInertiaSupported;   // 0x20
    dsSTRID fsmEvent;             // 0x24
    dsSTRID fsmTriggerEventOn;    // 0x28
    dsSTRID fsmTriggerEventOff;   // 0x2C
    bool    isForcedOnly;         // 0x30
    // trailing padding to 0x34 (52 bytes)

    aiSPD();                                                        // boundary ctor
    void ParsePS(const psSECTION *ps, const dsTSTRING<char> *name); // boundary

    // 0x83186130 (?IsEnabled@aiSPD@@QBA_NXZ) — true when this gait is usable: not forced-only, and
    // its effective max distance (tempMaxDist, or maxDist when tempMaxDist<0) is positive.
    // REVERSED: aiSPD__IsEnabled.cpp.
    bool IsEnabled() const;

    // 0x831852E8 (?Is@aiSPD@@QBA_NABVdsSTRID@@@Z) — true when this descriptor is named (name is a
    // valid interned id) and its name equals `_name`. REVERSED: aiSPD__Is.cpp.
    bool Is(const dsSTRID &_name) const;
    // 0x83185348 (?IsValid@aiSPD@@QBA_NXZ) — true when both `name` and `fsmEvent` are valid interned
    // ids. REVERSED: aiSPD__IsValid.cpp.
    bool IsValid() const;
    // 0x831853A0 (?IsValidTrigger@aiSPD@@QBA_NXZ) — true when the on/off FSM trigger events are
    // consistently paired (both empty or both set). REVERSED: aiSPD__IsValidTrigger.cpp.
    bool IsValidTrigger() const;

    // 0x8326A1D8 (?IsSameFsm@aiSPD@@QBA_NABV1@@Z) — true when this descriptor shares the same FSM
    // configuration as `other`: matching inertia support and identical fsmEvent / on / off trigger
    // ids. REVERSED: aiSPD__IsSameFsm.cpp.
    bool IsSameFsm(const aiSPD &other) const;
    // 0x8326A228 (?HaveTrigger@aiSPD@@QBA_NXZ) — true when both on and off FSM trigger events are
    // valid interned ids. REVERSED: aiSPD__HaveTrigger.cpp.
    bool HaveTrigger() const;
};
