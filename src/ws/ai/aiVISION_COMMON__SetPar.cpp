#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h" // pBrain->st.pointee->perc
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

// aiVISION_COMMON::SetPar @ 0x83291698
// ?SetPar@aiVISION_COMMON@@UAA_NABV?$dsTSTRING@D@@M@Z
// Virtual: apply a named vision tunable to the brain's perception senses (or this arm's eagle-eye
// flag). Straight/side sight cone angles and radii are kept mutually consistent (side angle >=
// straight angle; straight radius >= side radius). Returns true when the key was recognised.
bool aiVISION_COMMON::SetPar(const dsTSTRING<char> &key, float val)
{
    const char *k = key.pBuffer->str;
    aiSTATUS::PERC &perc = pBrain->st.pointee->perc;

    if (!strcmp(k, "angleSeeStraight"))
    {
        perc.senses.angleSeeStraight = val;
        if (perc.senses.angleSeeSide <= val)
            perc.senses.angleSeeSide = val;
        return true;
    }
    if (!strcmp(k, "angleSeeSide"))
    {
        if (perc.senses.angleSeeStraight >= val)
            perc.senses.angleSeeStraight = val;
        perc.senses.angleSeeSide = val;
        return true;
    }
    if (!strcmp(k, "radSeeStraight"))
    {
        if (perc.senses.radSeeSide >= val)
            perc.senses.radSeeSide = val;
        perc.senses.radSeeStraight = val;
        return true;
    }
    if (!strcmp(k, "radSeeSide"))
    {
        perc.senses.radSeeSide = val;
        if (perc.senses.radSeeStraight <= val)
            perc.senses.radSeeStraight = val;
        return true;
    }
    if (!strcmp(k, "isEagleEye"))
    {
        if (val <= 0.5f)
            state.val &= ~AI_VIST_SCR_EAGLE_EYE;
        else
            state.val |= AI_VIST_SCR_EAGLE_EYE;
        return true;
    }

    return false;
}
