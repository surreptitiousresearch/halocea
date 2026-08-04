#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiPERC_BASE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER.h"        // peekerSelf->timeInHOMode (aiWATCHER base @0x10)
#include "../../headers/ws/ds/dsSTRID.h"

// The interned float SSL parameter names (a8 AI namespace). boundary.
namespace AI {
    extern const dsSTRID GETF_HEALTH;               // ?GETF_HEALTH@AI@@2VdsSTRID@@B
    extern const dsSTRID GETF_DIST_TO_PLAYER;
    extern const dsSTRID GETF_DIST_TO_ENEMY;
    extern const dsSTRID GETF_DIST_TO_RETICLE;
    extern const dsSTRID GETF_TIME_STUCK_WAIT;
    extern const dsSTRID GETF_TIME_ENEMY_INVISIBLE;
    extern const dsSTRID GETF_TIME_ENEMY_IN_HO;
    extern const dsSTRID GETF_TIME_IN_HO;
    extern const dsSTRID GETF_PERCENT_ENEMY_VISIBLE;
    extern const dsSTRID GETF_ENEMY_TYPE;
    extern const dsSTRID GETF_ENEMY_DAMAGED;
}

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiBRAIN::GetFloatSSL(const dsSTRID&) @ 0x832AE630
// ?GetFloatSSL@aiBRAIN@@QAAMABVdsSTRID@@@Z
//
// Resolve an interned float SSL parameter `par` to its current value. Each supported name maps to a
// brain/mind/status query; an unknown name is logged and returns 0. (est bit 0x40 =
// AI_ENEST_WAS_VIS_BY_ME, bit 0x20 = AI_ENEST_VISIBLE; accessed via the raw flag word since AI_ENEST
// is only forward-declared here.)
float aiBRAIN::GetFloatSSL(const dsSTRID &par)
{
    if (AI::GETF_HEALTH.id == par.id)
        return GetHealthPercent();

    if (AI::GETF_DIST_TO_PLAYER.id == par.id)
        return GetDistToClosestPlayer();

    if (AI::GETF_DIST_TO_ENEMY.id == par.id)
        return mind.enemy.pointee->Is(nullptr) ? 2000000.0f : st.pointee->enemy.dist;

    if (AI::GETF_DIST_TO_RETICLE.id == par.id)
        return st.pointee->perc.distToReticle;

    if (AI::GETF_TIME_STUCK_WAIT.id == par.id)
        return st.pointee->radar.timeWaiting;

    if (AI::GETF_TIME_ENEMY_INVISIBLE.id == par.id)
    {
        if (st.pointee->est.val & 0x40) // AI_ENEST_WAS_VIS_BY_ME
            return mind.enemy.pointee->GetTimeInvisible();
        return 0.0f;
    }

    if (AI::GETF_TIME_ENEMY_IN_HO.id == par.id)
        return mind.enemy.pointee->GetTimeInHO();

    if (AI::GETF_TIME_IN_HO.id == par.id)
    {
        aiWATCHER_BRAIN *peekerSelf = st.pointee->peekerSelf;
        if (peekerSelf)
            return reinterpret_cast<aiWATCHER *>(peekerSelf)->timeInHOMode;
        return 0.0f;
    }

    if (AI::GETF_PERCENT_ENEMY_VISIBLE.id == par.id)
    {
        if (st.pointee->est.val & 0x20) // AI_ENEST_VISIBLE
            return 100.0f;
        return 0.0f;
    }

    if (AI::GETF_ENEMY_TYPE.id == par.id)
        return GetEnemyType_SSL();

    if (AI::GETF_ENEMY_DAMAGED.id == par.id)
        return mind.perc.pointee->GetFloat();

    _apLog("~AI,Ssl,Error~'%s' GetFloat(\"%s\"): unknown parameter name",
           names.nameInst, par.CStr());
    return 0.0f;
}
