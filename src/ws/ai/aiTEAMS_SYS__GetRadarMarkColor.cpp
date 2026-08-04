// aiTEAMS_SYS::GetRadarMarkColor @ 0x831731F0
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/m3d/m3dCOLOR.h"
#include "../../headers/ws/gs/gsTEAMS.h"

// Radar blip colour (sret m3dCOLOR): red when `team` is an enemy of the player,
// green otherwise; alpha 255. DEVIATION: the decompiler's return-by-value m3dCOLOR is
// materialised through the sret pointer (IDA's "this"); modelled here as `result`.
m3dCOLOR *aiTEAMS_SYS::GetRadarMarkColor(m3dCOLOR *result, const propGAME_INFO_TEAM &team) const
{
    dsSTRID teamID;
    teamID.id = team.team.id;
    dsSTRID players;
    const dsSTRID *playersID = aiTEAMS_SYS::PlayersTeamID(&players);
    gsTEAMS::ATTITUDE attitude = gsTEAMS::Instance().GetAttitude(teamID, *playersID);

    result->a = 255.0f;
    if (attitude != gsTEAMS::ATT_ENEMY) {
        result->r = 70.0f;
        result->g = 211.0f;
        result->b = 70.0f;
    } else {
        result->r = 232.0f;
        result->g = 0.0f;
        result->b = 0.0f;
    }
    return result;
}
