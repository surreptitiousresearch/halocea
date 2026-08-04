#pragma once
// ws-engine ai08: a small phase state-machine driver. Tracks the current phase (an enum),
// the countdown until the next phase change, and the constant default phase to fall back to.
// DB-verified layout (types_members aiPHASER<enum aiWEAPON::aiS_PHASES>): phase@0,
// timeToChangePhase@4, idDefPhase@8 — size 12.

template<class PHASE>
struct aiPHASER {
    PHASE       phase;             // 0x00 current phase
    float       timeToChangePhase; // 0x04 countdown until auto-transition
    const PHASE idDefPhase;        // 0x08 default/idle phase
};
