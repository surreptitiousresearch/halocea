#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/wpn/atkDSP_INFO.h"

// aiWEAPON::CalcHeightAddition @ 0x83293108
// ?CalcHeightAddition@aiWEAPON@@IBAMABUm3dV@@ABVatkDSP_INFO@@PBUwpnS_DESC@@@Z
//
// Core parabolic-aim solver: given the target, an attack's speed/accel, and the strike descriptor,
// compute the extra vertical aim offset needed to lob a projectile onto `trg`. Returns 0 for a
// linear-trajectory weapon, when acceleration is negligible, or when the muzzle tip can't be found.
float aiWEAPON::CalcHeightAddition(const m3dV &trg, const atkDSP_INFO &dspInfo,
                                   const wpnS_DESC *desc) const
{
    if (!desc->typeTraj) // trajLINEAR == 0
        return 0.0f;

    float accel = dspInfo.accel;
    float speed = dspInfo.speed;
    if (__fabs(accel) < 0.000001f)
        return 0.0f;

    m3dV posTip = m3dVZero;
    m3dV dirTip;
    if (!GetPosDirTip(posTip, dirTip))
        return 0.0f;

    aiSTATUS *st = pBrain->st.pointee;

    // Displacement to the target, scaled by the descriptor's fudge factor.
    m3dV rel = trg - posTip;
    rel *= desc->BRUTE_UGLY_HACK;

    // Split rel into its component along the enemy's up axis and the remaining horizontal reach.
    const m3dV &up = st->enemy.cs.dirUpBody;
    float along = up.x * rel.x + up.y * rel.y + up.z * rel.z;
    m3dV horiz = rel - up * along;
    float horizLen = m3dLengthVector(&horiz) + 0.0099999998f;

    _m3dCheckValid(speed);
    float speedSq = speed * speed;
    _m3dCheckValid(speedSq);
    _m3dCheckValid(horizLen);

    float disc = speedSq * speedSq
               - (horizLen * horizLen * accel + speedSq * along * 2.0f) * accel;
    if (disc >= -0.000001f) {
        if (__fabs(disc) < 0.000001f)
            return speedSq / accel - along;
        float sign = desc->paramsParabolic.isLowTrajectory ? -1.0f : 1.0f;
        _m3dCheckValid(disc);
        horizLen = (__fsqrts(disc) * sign + speedSq) / accel;
    }
    return horizLen - along;
}
