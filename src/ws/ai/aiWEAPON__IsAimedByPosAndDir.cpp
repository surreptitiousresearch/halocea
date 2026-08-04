#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiWEAPON::IsAimedByPosAndDir @ 0x83295660
// ?IsAimedByPosAndDir@aiWEAPON@@IBA_NABUm3dV@@ABUaiSTRIKE@@@Z
//
// Is the strike's muzzle aimed at `posTarget`? Fetch the tip pos/dir, then compare the fire
// direction with the direction from tip to (target + fireDir). For aim-check mode 1 the comparison
// is done in the body's horizontal plane (up component removed). Aimed when the cosine exceeds the
// descriptor's coeffAim.
bool aiWEAPON::IsAimedByPosAndDir(const m3dV &posTarget, const aiSTRIKE &strike) const
{
    m3dV posTip = m3dVZero;
    m3dV dirTip = m3dVZero;
    if (!GetPosDirTip(strike, posTip, dirTip))
        return false;

    m3dNormalize(&dirTip);
    _m3dCheckValid(&posTarget);
    _m3dCheckValid(&dirTip);

    m3dV posAimed; // posTarget + dirTip
    posAimed.x = posTarget.x + dirTip.x;
    posAimed.y = posTarget.y + dirTip.y;
    posAimed.z = posTarget.z + dirTip.z;
    _m3dCheckValid(&posAimed);
    _m3dCheckValid(&posTip);

    const wpnS_DESC *desc = strike.desc;
    m3dV dirToTarget; // posAimed - posTip
    dirToTarget.x = posAimed.x - posTip.x;
    dirToTarget.y = posAimed.y - posTip.y;
    dirToTarget.z = posAimed.z - posTip.z;

    int typeIsAimed = desc->typeIsAimed;
    if (typeIsAimed == 4)
        typeIsAimed = (int)GetAimCheck(strike.vars->tie.idxCtrl);

    if (typeIsAimed == 1) {
        const m3dV *dirUpBody = &pBrain->st.pointee->cs.dirUpBody;
        m3dTangentComponent(&dirToTarget, dirUpBody, &dirToTarget);
        m3dTangentComponent(&dirTip, dirUpBody, &dirTip);
    }

    float coeffAim = strike.desc->coeffAim;
    float cosAngle = m3dAngleVector_COS(&dirTip, &dirToTarget);
    return cosAngle > coeffAim;
}
