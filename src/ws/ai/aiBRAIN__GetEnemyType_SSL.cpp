#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"          // mind.enemy
#include "../../headers/ws/ai/aiSTATUS.h"        // st.pointee->est
#include "../../headers/ws/ai/aiENEMY_BASE.h"    // enemy module query surface
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"   // GetTeamID
#include "../../headers/ws/gs/gsTEAMS.h"          // gsTEAMS::NeutralTeamID
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiBRAIN::GetEnemyType_SSL() @ 0x832ADAB8
// ?GetEnemyType_SSL@aiBRAIN@@QBAMXZ
//
// Classify this brain's current enemy for the SSL layer:
//   0.0  = no enemy
//   0.5  = neutral (enemy-est bit 19 set, and enemy is on the neutral team or shares this team)
//   3.0  = other/opposing team (enemy-est bit 19 set, not same team)
//   2.0  = the enemy is the player
//   1.0  = the enemy is an AI brain or a plain entity
//  -1.0  = otherwise
float aiBRAIN::GetEnemyType_SSL() const
{
    aiENEMY_BASE *enemy = mind.enemy.pointee;
    propGAME_INFO_TEAM *myTeam = GetTeamID();
    bool sameTeam = (enemy->GetTeamID()->team.id == myTeam->team.id);

    if (enemy->Is(nullptr))
        return 0.0f;

    bool estNeutralCapable = ((st.pointee->est.val >> 19) & 1) != 0;
    if (estNeutralCapable)
    {
        dsSTRID neutral;
        const char *neutralId = gsTEAMS::NeutralTeamID(&neutral)->id;
        if (neutralId == enemy->GetTeamID()->team.id || sameTeam)
            return 0.5f;
    }
    if (estNeutralCapable && !sameTeam)
        return 3.0f;

    if (enemy->IsPlayer())
        return 2.0f;
    if (enemy->IsBrain() || enemy->IsEntity())
        return 1.0f;
    return -1.0f;
}
