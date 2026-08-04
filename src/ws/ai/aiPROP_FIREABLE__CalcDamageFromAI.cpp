#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"
#include "../../headers/ws/m3d/m3dSPL_LINEAR1D.h"

// aiPROP_FIREABLE::CalcDamageFromAI(const dmgDAMAGE*, aiBRAIN*) const @ 0x832A3810
// ?CalcDamageFromAI@aiPROP_FIREABLE@@IBAMPBVdmgDAMAGE@@PAVaiBRAIN@@@Z (protected const)
// Scale the raw damage amount by the shooter's script-defined distance-vs-damage curve.
float aiPROP_FIREABLE::CalcDamageFromAI(const dmgDAMAGE *pDmg, aiBRAIN *shooter) const
{
    m3dSPL_LINEAR1D *curveDamage = &shooter->mind.scr.curveDamage;

    float dist = shooter->Dist(&pDmg->params.orgDmg);
    float scale;
    curveDamage->GetValue(dist, &scale, nullptr);

    return pDmg->amount * scale;
}
