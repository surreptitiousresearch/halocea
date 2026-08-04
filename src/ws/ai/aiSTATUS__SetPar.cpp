#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

// aiSTATUS::SetPar @ 0x83228500
// ?SetPar@aiSTATUS@@QAA_NABV?$dsTSTRING@D@@M@Z
//
// Set a named float/bool tunable on this per-brain status block. Each recognised key maps to a
// specific status-flag bit (in gen/body/mind/die) or a scalar field; the value is interpreted as a
// boolean via a 0.5 threshold for flags, or stored directly for scalars. Returns true when the key
// was recognised, false otherwise. The bit numbers / clear-vs-set polarity below are disasm-exact.
bool aiSTATUS::SetPar(const dsTSTRING<char> &key, float val)
{
    const char *k = key.pBuffer->str;

    if (!strcmp(k, "isAggressive"))
    {
        if (val <= 0.5f) mind.val &= ~0x10; else mind.val |= 0x10;
        return true;
    }
    if (!strcmp(k, "isSkipDestr"))
    {
        if (val <= 0.5f) body.val &= ~0x800000; else body.val |= 0x800000;
        return true;
    }
    if (!strcmp(k, "isAcceptFrFire"))
    {
        if (val <= 0.5f) gen.val &= ~0x200000; else gen.val |= 0x200000;
        return true;
    }
    if (!strcmp(k, "timeStuckWait"))
    {
        radar.paramWait = val;
        return true;
    }
    if (!strcmp(k, "isForceHOPath"))
    {
        if (val <= 0.5f) gen.val &= ~0x400000; else gen.val |= 0x400000;
        return true;
    }
    if (!strcmp(k, "isDisablePathOptimization"))
    {
        if (val <= 0.5f) gen.val &= ~0x200; else gen.val |= 0x200;
        return true;
    }
    if (!strcmp(k, "failPathBhvOnStuck"))
    {
        if (val <= 0.5f) gen.val &= ~0x1000; else gen.val |= 0x1000;
        return true;
    }
    if (!strcmp(k, "avoid3DHOPaths"))
    {
        if (val <= 0.5f) gen.val &= ~0x10000; else gen.val |= 0x10000;
        return true;
    }
    if (!strcmp(k, "enableHOOn3DHOPath"))
    {
        if (val <= 0.5f) gen.val &= ~0x20000; else gen.val |= 0x20000;
        return true;
    }
    if (!strcmp(k, "disable3DHOMeleePoints"))
    {
        if (val <= 0.5f) gen.val &= ~0x40000; else gen.val |= 0x40000;
        return true;
    }
    if (!strcmp(k, "force3DHONearEnemy"))
    {
        if (val <= 0.5f) gen.val &= ~0x80000; else gen.val |= 0x80000;
        return true;
    }
    if (!strcmp(k, "isReactToReticle"))
    {
        if (val <= 0.5f) gen.val &= ~0x4000; else gen.val |= 0x4000;
        return true;
    }
    if (!strcmp(k, "canAvoidGrenade"))
    {
        if (val <= 0.5f) gen.val &= ~0x8000; else gen.val |= 0x8000;
        return true;
    }
    if (!strcmp(k, "radarEnable"))
    {
        // radar-enable clears its two flags when the value is truthy (>=0.5).
        if (val >= 0.5f) gen.val &= ~0x100; else gen.val |= 0x100;
        if (val >= 0.5f) body.val &= ~0x100000; else body.val |= 0x100000;
        return true;
    }
    if (!strcmp(k, "mayGoAroundBlocker"))
    {
        if (val >= 0.5f) gen.val &= ~0x400; else gen.val |= 0x400;
        return true;
    }
    if (!strcmp(k, "isMeleeUnit"))
    {
        if (val <= 0.5f) mind.val &= ~0x10000; else mind.val |= 0x10000;
        return true;
    }
    if (!strcmp(k, "smtDeathMaxDist"))
    {
        smtDeathMaxDist = val;
        return true;
    }
    if (!strcmp(k, "isEnableSmtDead"))
    {
        if (val <= 0.5f) die.val &= ~0x1000; else die.val |= 0x1000;
        return true;
    }
    if (!strcmp(k, "isDropItems"))
    {
        if (val >= 0.5f) die.val &= ~0x4000; else die.val |= 0x4000;
        return true;
    }
    if (!strcmp(k, "contourShooting"))
    {
        aim.contourShooting.isEnabled = val > 0.5f;
        return true;
    }
    if (!strcmp(k, "MayMeleeInSNS"))
    {
        if (val <= 0.5f) mind.val &= ~0x40000; else mind.val |= 0x40000;
        return true;
    }
    if (!strcmp(k, "KeepPosWhileStay"))
    {
        if (val <= 0.5f) mind.val &= ~0x8000; else mind.val |= 0x8000;
        return true;
    }
    if (!strcmp(k, "delayOnAlertMin"))
    {
        if (val <= 0.0f) val = 0.0f;
        float prevMax = perc.delayOnAlertMax;
        perc.delayOnAlertMin = val;
        if (prevMax < val)
            perc.delayOnAlertMax = val; // keep max >= min
        return true;
    }
    if (!strcmp(k, "delayOnAlertMax"))
    {
        perc.delayOnAlertMax = (val > 0.0f) ? val : 0.0f;
        return true;
    }
    return false;
}
