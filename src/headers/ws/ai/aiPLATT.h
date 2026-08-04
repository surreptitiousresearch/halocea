#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/WEAK_PTR.h"

// ws-engine ai08 — the "player attackers" registry (aiPLANNER::platt): the set of brains currently
// attacking the local player, capped at maxAttackersCount. DB-verified layout (types_members
// aiPLATT) — size 24.

struct aiBRAIN; // ai08 — per-unit AI brain  boundary (ptr)

struct aiPLATT {
    dsVECTOR<ds::WEAK_PTR<aiBRAIN>, 8> attackers;        // 0x00 brains attacking the player
    int                                maxAttackersCount; // 0x14 cap

    // ?AddAttacker@aiPLATT@@QAAXPBVaiBRAIN@@@Z — register `brain` as attacking the player. boundary.
    void AddAttacker(const aiBRAIN *brain);
    // ?RemoveAttacker@aiPLATT@@QAAXPBVaiBRAIN@@@Z — drop `brain` from the attacker set. boundary.
    void RemoveAttacker(const aiBRAIN *brain);
    // ?ProcessFrame@aiPLATT@@QAAXXZ — prune attackers whose current enemy is no longer the player.
    void ProcessFrame();
};
