#include "../../headers/ws/ai/aiBRAIN_DESC_parse_boundary.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h" // aiGOAL_BASE::ParseSpline
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN_DESC::ReadEnemy(const psSECTION&) @ 0x83187420
// Read the enemy-selection / firing-error-model / HO-shift tuning block. Each scalar defaults to
// its current value; the aim-shift response curve defaults to a flat "0;0|888;0" spline.
void aiBRAIN_DESC::ReadEnemy(const psSECTION &psReadS)
{
    const psSECTION_INTERFACE &psRead = static_cast<const psSECTION_INTERFACE &>(psReadS);

    distEnemyPlayerOnly     = psRead.GetFloat("distEnemyPlayerOnly", distEnemyPlayerOnly, 0);
    maxMeleeEnemies         = psRead.GetInt("countMaxMeleeEnemies", maxMeleeEnemies, 0);
    maxRangedEnemies        = psRead.GetInt("countMaxRangedEnemies", maxRangedEnemies, 0);
    coefFER_dist_PlrOpened  = psRead.GetFloat("coefFER_dist_PlrOpened", coefFER_dist_PlrOpened, 0);
    coefFER_dist_PlrCovered = psRead.GetFloat("coefFER_dist_PlrCovered", coefFER_dist_PlrCovered, 0);
    coefFER_dist_AiOpened   = psRead.GetFloat("coefFER_dist_AiOpened", coefFER_dist_AiOpened, 0);
    coefFER_dist_AiCovered  = psRead.GetFloat("coefFER_dist_AiCovered", coefFER_dist_AiCovered, 0);
    coefFEM_dist_Plr        = psRead.GetFloat("coefFEM_dist_Plr", coefFEM_dist_Plr, 0);
    coefFEM_dist_AI         = psRead.GetFloat("coefFEM_dist_AI", coefFEM_dist_AI, 0);
    shiftQuarterOrigDir     = psRead.GetFloat("shiftQuarterOrigDir", shiftQuarterOrigDir, 0);
    shiftQuarterOrigUp      = psRead.GetFloat("shiftQuarterOrigUp", shiftQuarterOrigUp, 0);
    shiftQuarterOrigRight   = psRead.GetFloat("shiftQuarterOrigRight", shiftQuarterOrigRight, 0);
    shiftVisionPosInHO      = psRead.GetFloat("shiftVisionPosInHO", shiftVisionPosInHO, 0);
    shiftPosFaceHorInHighHO = psRead.GetFloat("shiftPosFaceHorInHighHO", shiftPosFaceHorInHighHO, 0);
    shiftPosFaceVertInHighHO = psRead.GetFloat("shiftPosFaceVertInHighHO", shiftPosFaceVertInHighHO, 0);

    if (psRead.IsKeyExist("shiftAimPosInLowHO", 0))
    {
        // Lazily-constructed default response curve (flat: constant 0 out to 888).
        static dsTSTRING<char> defaultShiftAimPosInLowHOCurve("0;0|888;0");
        dsTSTRING<char> curve = psRead.GetStr("shiftAimPosInLowHO",
                                              defaultShiftAimPosInLowHOCurve, 0);
        aiGOAL_BASE::ParseSpline(&curve, &shiftAimPosInLowHO);
    }
}
