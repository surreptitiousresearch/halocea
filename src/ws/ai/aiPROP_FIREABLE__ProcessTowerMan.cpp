#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/m3d/m3dV.h"
#include <math.h>

// aiPROP_FIREABLE::ProcessTowerMan() @ 0x832A4788
// ?ProcessTowerMan@aiPROP_FIREABLE@@IAA_NXZ  (protected)
//
// If this unit is a "tower-man" (dies by toppling toward a linked waypoint), pick the target
// waypoint, ragdoll the body, and apply a directed impulse scaled by both the unit's and the
// waypoint's tower-man strength. Returns true when a topple was applied.
bool aiPROP_FIREABLE::ProcessTowerMan()
{
    aiBRAIN *pOwner = (aiBRAIN *)this->pOwner;
    if (!pOwner)
        return false;

    float strength = aibrain_TowerManStrength(this->pOwner);
    if (strength < 0.000001f)
        return false;

    static float maxDist_2 = 4.0f; // guarded-init module global in the binary

    navWP *wp = this->FindTowerManWPwoNS(pOwner, maxDist_2);
    if (!wp)
    {
        wp = this->FindTowerManWPbyNS(pOwner, maxDist_2);
        if (!wp)
            return false;
    }

    float wpStrength = wpWP_GetTwrManStrgth(wp->wpData);

    m3dV dir;
    if (!wp->IsHO() || wpWP_HaveChilds(wp->wpData))
    {
        navWP_GetDir(wp, &dir);
    }
    else
    {
        const m3dV *dirToHO = wp->GetDirToHOPlane();
        dir = *dirToHO;
    }

    if (wpStrength < 0.000001f || fabsf(m3dLengthVector_2(&dir)) < M3D_EPSILON_2_633)
        return false;

    m3dNormalize(&dir);
    dir *= wpStrength * strength;

    aiBODY_IFACE *pBody = pOwner->body.pointee;
    pBody->SendFsmEventByName_2(fsmRAGDOLL_EVENT_start());

    pBody = pOwner->body.pointee;
    m3dV impulse = dir;
    pBody->SendFsmEventByName(fsmPHYS_EVENT_apply_speed(), &impulse);
    return true;
}
