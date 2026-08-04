#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h" // _m3dCheckValid
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::IsNeededTargetAlongWpn @ 0x83296140
// ?IsNeededTargetAlongWpn@aiWEAPON@@IBA_NXZ
//
// True when the module must aim a target explicitly along the weapon: a strike is selected, the mind
// is not in the suppressing state (bit 9), and the brain's commanded aim point is not already aimed.
bool aiWEAPON::IsNeededTargetAlongWpn() const
{
    if (tieSelected.IsNull())
        return false;

    if ((pBrain->st.pointee->mind.val >> 9) & 1)
        return false;

    m3dV aimPoint = pBrain->cmd.posAim;
    _m3dCheckValid(&aimPoint);
    return !IsAimed(aimPoint, tieSelected);
}
