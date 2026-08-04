#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ent/entENTITY.h"

// ?GetTeamIDFromEntity@@YA?AVdsSTRID@@PAVentENTITY@@@Z @ 0x83173B48 — the interned team id of an
// entity (sret). boundary — body external to this batch.
dsSTRID GetTeamIDFromEntity(entENTITY *ent);

// aiPERC::NoticeVictimDamaged @ 0x8329CCD8
// ?NoticeVictimDamaged@aiPERC@@UAAXPBVentENTITY@@M_N@Z
//
// A shot the unit fired hit `victim` for `amount` (and possibly killed it). If the victim is an
// enemy team, accumulate the damage/kill toward the next OnEnemyDamaged report (unless still within
// the accumulation lock-out); otherwise reset the accumulator and re-arm a short lock-out.
void aiPERC::NoticeVictimDamaged(const entENTITY *victim, float amount, bool isKilled)
{
    dsSTRID victimTeam = GetTeamIDFromEntity(const_cast<entENTITY *>(victim));
    // aiBRAIN_IFACE::GetTeamID()->team is a dsSTRID; reinterpret its interned id pointer as the
    // one-word dsSTRID the planner expects (matches the disassembly's &id argument).
    const char *selfTeamId = pBrain->GetTeamID()->team.id;
    const dsSTRID &selfTeam = reinterpret_cast<const dsSTRID &>(selfTeamId);

    if (aiPlanner->IsEnemy(selfTeam, victimTeam)) {
        if (infliction.timerLocked <= 0.000001f) {
            infliction.dmgAccumulated += amount;
            infliction.killed += isKilled ? 1 : 0;
        }
    } else {
        infliction.killed = 0;
        infliction.dmgAccumulated = 0.0f;
        infliction.timerLocked = 0.2f;
    }
}
