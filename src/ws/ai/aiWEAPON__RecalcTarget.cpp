#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h" // m3dVZero, _m3dCheckValid, operator*
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::RecalcTarget @ 0x83296220
// ?RecalcTarget@aiWEAPON@@IAAXXZ
//
// When the weapon needs an explicit along-barrel target, project a point 10 units ahead of the
// muzzle tip along the firing direction and store it as the brain's commanded weapon target.
void aiWEAPON::RecalcTarget()
{
    if (!IsNeededTargetAlongWpn())
        return;

    m3dV posTip = m3dVZero;
    m3dV dirTip = m3dVZero;
    GetPosDirTip(posTip, dirTip); // virtual (current-strike) tip query

    m3dV ahead = dirTip * 10.0f;
    _m3dCheckValid(&posTip);
    _m3dCheckValid(&ahead);

    m3dV &wpnTgt = pBrain->cmd.posWpnTgt;
    wpnTgt.x = ahead.x + posTip.x;
    wpnTgt.y = ahead.y + posTip.y;
    wpnTgt.z = ahead.z + posTip.z;
}
