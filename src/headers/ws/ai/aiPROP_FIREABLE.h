#pragma once
#include "../prop/propFIREABLE.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08: the AI-side fireable/health component (a8_prop_fireable.cpp). Extends the shared
// propFIREABLE with AI death behaviour: revitalize-skip tracking, "tower-man" ragdoll toppling, and
// damage scaling / kill-event dispatch through the owning aiBRAIN.
// DB-verified layout (headers_ref aiPROP_FIREABLE, size 0x148): propFIREABLE base@0, then
// isSkipRevitalize@0x144.

struct dmgTYPE;
struct dmgDAMAGE;
struct gsGEOM_ANCHOR;
struct aiBRAIN;
struct navWP;

struct aiPROP_FIREABLE : propFIREABLE {
    bool isSkipRevitalize; // 0x144 suppress the next revitalize-on-death

    // 0x832A3B88 (?IsAcceptDamage@aiPROP_FIREABLE@@UAA_NPBVdmgTYPE@@ABVgsGEOM_ANCHOR@@H@Z) — vtbl:
    // god-mode + descriptor accept test, then the per-object fireable filter.  REVERSED.
    bool IsAcceptDamage(const dmgTYPE *pDmgType, const gsGEOM_ANCHOR &hitInfo, int isDirectHit);

    // 0x832A3C30 (?CalcDamageFromPlayer@aiPROP_FIREABLE@@IBAMPBVdmgDAMAGE@@@Z) — protected const:
    // scale down player-sourced friendly fire to a token amount.  REVERSED.
    float CalcDamageFromPlayer(const dmgDAMAGE *pDmg) const;

    // 0x832A3DA8 (?CalcDamageToApply@aiPROP_FIREABLE@@IBAMPBVdmgDAMAGE@@@Z) — protected const:
    // resolve the effective damage (bleed/revitalize gating, AI-vs-AI distance scaling, player
    // friendly-fire).  REVERSED.
    float CalcDamageToApply(const dmgDAMAGE *pDmg) const;

    // 0x832A3F88 (?Die@aiPROP_FIREABLE@@UAAXABV?$dsTSTRING@D@@@Z) — vtbl: scripted death by kill-word
    // sequence (REMOVE/RAGDOLL/HIDE/PIERCE/MIND/KEEP/BURN/WATER), applies a self-damage impulse.
    // REVERSED.
    void Die(const dsTSTRING<char> &nameSeq);

    // 0x832A44F8 (?FindTowerManWPbyNS@aiPROP_FIREABLE@@IAAPAVnavWP@@PAVaiBRAIN@@M@Z) — protected:
    // best tower-man waypoint within `maxDist` via the brain's nav-system weight graph.  REVERSED.
    navWP *FindTowerManWPbyNS(aiBRAIN *pBrain, float maxDist);

    // 0x832A4680 (?FindTowerManWPwoNS@aiPROP_FIREABLE@@IAAPAVnavWP@@PAVaiBRAIN@@M@Z) — protected:
    // nearest tower-man waypoint within `maxDist` from the planner's global tower-man list.  REVERSED.
    navWP *FindTowerManWPwoNS(aiBRAIN *pBrain, float maxDist);

    // 0x832A4788 (?ProcessTowerMan@aiPROP_FIREABLE@@IAA_NXZ) — protected: if this unit is a tower-man,
    // topple it toward the chosen waypoint (ragdoll + directed impulse).  REVERSED.
    bool ProcessTowerMan();

    // 0x832A4A30 (?SendDamage@aiPROP_FIREABLE@@UAAXPAVdmgDAMAGE@@@Z) — vtbl: apply effective damage,
    // forward to base, and fire the body hit FSM event.  REVERSED.
    void SendDamage(dmgDAMAGE *pDmg);

    // 0x832A4AC0 (?Die@aiPROP_FIREABLE@@MAAXPBVdmgDAMAGE@@@Z) — protected virtual: full death handling
    // (bleed vs death SSL events, kill notification, tower-man topple, body die FSM).  REVERSED.
    void Die(const dmgDAMAGE *pDmg);

    // 0x832A4F58 (?AcceptDamage@aiPROP_FIREABLE@@MAAXPBVdmgDAMAGE@@@Z) — protected virtual: entry point
    // for an incoming hit (ignore/shoot-through gating, base accept, precision-stat + wounded notify).
    // REVERSED.
    void AcceptDamage(const dmgDAMAGE *pDmg);

    // 0x832A39C8 (?IsIgnoring@aiPROP_FIREABLE@@IBA_NPBVdmgDAMAGE@@@Z) — protected const: whether this
    // unit should ignore `pDmg` (god-mode, non-explosive friendly fire, same-team no-FF).
    // REVERSED: src/ws/ai/aiPROP_FIREABLE__IsIgnoring.cpp.
    bool IsIgnoring(const dmgDAMAGE *pDmg) const;

    // 0x832A38D0 (?Revitalize@aiPROP_FIREABLE@@UAAXXZ) — restore from downed: base revitalize, clear
    // the owner's BLEED death bit, reset the model's collision layer.
    // REVERSED: src/ws/ai/aiPROP_FIREABLE__Revitalize.cpp.
    void Revitalize();

    // 0x832A3810 (?CalcDamageFromAI@aiPROP_FIREABLE@@IBAMPBVdmgDAMAGE@@PAVaiBRAIN@@@Z) — protected
    // const: scale the raw damage by the shooter's distance-vs-damage curve.
    // REVERSED: src/ws/ai/aiPROP_FIREABLE__CalcDamageFromAI.cpp.
    float CalcDamageFromAI(const dmgDAMAGE *pDmg, aiBRAIN *shooter) const;

    // 0x832A3958 (?AcceptDmgLog@aiPROP_FIREABLE@@MAAXPBVdmgDAMAGE@@@Z) — protected virtual: record a
    // damage event in the AI damage log. REVERSED: src/ws/ai/aiPROP_FIREABLE__AcceptDmgLog.cpp.
    void AcceptDmgLog(const dmgDAMAGE *pDmg);

    // 0x832A3B08 (?ProcessINIT@aiPROP_FIREABLE@@UAA_NXZ) — per-spawn init; asserts the owner is an
    // aiBRAIN, then chains to the base. REVERSED: src/ws/ai/aiPROP_FIREABLE__ProcessINIT.cpp.
    bool ProcessINIT();
};
