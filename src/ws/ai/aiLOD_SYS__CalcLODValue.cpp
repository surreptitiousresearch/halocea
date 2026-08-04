#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // m3dVZero
#include "../../headers/ws/m3d/m3d_boundary.h"              // m3dDist / _m3dCheckValid
#include "../../headers/ws/m3d/m3dV.h"

// aiLOD_SYS::CalcLODValue @ 0x83238E78
// ?CalcLODValue@aiLOD_SYS@@IBAMPAVaiWATCHER_BRAIN@@PAVaiWATCHER@@@Z
//
// The LOD value of one AI unit relative to one player watcher: map the player-to-unit distance
// through the LOD response spline; then, if the player is facing away from the unit (dot of the
// player's body-forward with the player->unit vector is negative), bump the value by one LOD step
// (less detail behind the player's back).
float aiLOD_SYS::CalcLODValue(aiWATCHER_BRAIN *pBrainWatcher, aiWATCHER *player) const
{
    m3dV playerPos = m3dVZero;
    m3dV playerFwd = m3dVZero;
    m3dV playerUp; // filled but unused by this function
    player->GetBodyCS(&playerPos, &playerFwd, &playerUp);

    // vtbl+0x34: aiWATCHER_BRAIN::GetBodyPosNav — the unit's nav-space body position.
    m3dV unitPos = pBrainWatcher->GetBodyPosNav();

    float dist = m3dDist(&playerPos, &unitPos);

    // GetValue's decompiler cast is wrong (PPC float/vararg mis-read); the DB mangling is
    // void GetValue(float t, void* outValue, void* outAux). Only outValue is consumed here.
    float lodValue[2] = { 0.0f, 0.0f };
    curveLODValue.GetValue(dist, &lodValue[0], &lodValue[1]);

    _m3dCheckValid(&unitPos);
    _m3dCheckValid(&playerPos);

    float facing = playerFwd.y * (unitPos.y - playerPos.y)
                 + (unitPos.x - playerPos.x) * playerFwd.x
                 + playerFwd.z * (unitPos.z - playerPos.z);

    if (facing >= 0.0f)
        return lodValue[0];
    return lodValue[0] + 1.0f;
}
