#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"
#include "../../headers/ws/wpn/atkDSP_INFO.h"

// aiWEAPON::RecalcVars @ 0x83293398
// ?RecalcVars@aiWEAPON@@IBAXAAUaiSTRIKE@@@Z
//
// Recompute a strike's parabolic prediction and launch speed. Linear-trajectory or invalid attacks
// zero both; otherwise solve for the two-phase (up/down) ballistic arc that reaches the chosen aim
// point, then derive the muzzle speed from the horizontal reach and vertical component.
void aiWEAPON::RecalcVars(aiSTRIKE &strike) const
{
    if (!strike.vars)
        return;
    const wpnS_DESC *desc = strike.desc;
    if (!desc)
        return;
    if (desc->typeTraj == trajLINEAR) {
        strike.vars->parabolicPrediction = 0.0f;
        strike.vars->startSpeed = 0.0f;
        return;
    }

    atkDSP_INFO info;
    float accel;
    if (!strike.wpn->GetAttackInfo(strike.desc->atkID, &info)
        || (*(const unsigned char *)&info & 0x40) == 0
        || (accel = info.accel, info.accel < 0.000001f)
        || strike.desc->paramsParabolic.heightMax < 0.000001f) {
        strike.vars->parabolicPrediction = 0.0f;
        strike.vars->startSpeed = 0.0f;
        return;
    }

    aiS_VARS *vars = strike.vars;
    aiSTATUS *st = pBrain->st.pointee;

    m3dV posTip = m3dVZero;
    m3dV dirTip = m3dVZero;
    GetPosDirTip(vars->tie.idxCtrl, posTip, dirTip);

    // Pick the aim point according to the descriptor's aim-point type.
    m3dV aimPos = st->aim.pointForProjectile;
    switch (strike.desc->typeAimPos) {
        case STRIKE_MODIFIERS::AIM_GRENADE:
            aimPos = st->enemy.posVisLast;
            break;
        case STRIKE_MODIFIERS::AIM_SNIPER:
            aimPos = st->enemy.visResult.points.posBottom;
            break;
        default:
            break;
    }
    if (m3dIsEqualPoint_A(&aimPos, &m3dVUndef, 0.000001f)) {
        strike.vars->parabolicPrediction = 0.0f;
        strike.vars->startSpeed = 0.0f;
        return;
    }

    m3dV rel = aimPos - posTip;
    const m3dV &up = st->cs.dirUpBody;
    float along = up.x * rel.x + up.y * rel.y + up.z * rel.z;
    m3dV horiz = rel - up * along;
    float horizLen = m3dLengthVector(&horiz);

    float heightMax = strike.desc->paramsParabolic.heightMax;
    if (heightMax <= along)
        heightMax = along;

    _m3dCheckValid(heightMax * accel * 2.0f);
    float vUp = __fsqrts(heightMax * accel * 2.0f);
    float downTerm = (heightMax - along) * accel * 2.0f;
    _m3dCheckValid(downTerm);
    float vDown = __fsqrts(downTerm);

    float timeTotal = (vDown + vUp) / accel;
    float vHoriz = horizLen / timeTotal;
    strike.vars->parabolicPrediction = timeTotal * vUp - along;

    float speedSq = vHoriz * vHoriz + vUp * vUp;
    _m3dCheckValid(speedSq);
    strike.vars->startSpeed = __fsqrts(speedSq);
}
