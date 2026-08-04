#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

// aiPERC::SetPar @ 0x8329CA90
// ?SetPar@aiPERC@@UAA_NABV?$dsTSTRING@D@@M@Z
//
// Apply a named perception tunable to the brain's status block. Recognises the three hearing radii
// and the "isYieldWay" mind flag (bit 0x10000000). Returns true when the key was consumed.
bool aiPERC::SetPar(const dsTSTRING<char> &key, float val)
{
    aiSTATUS *st = pBrain->st.pointee;
    const char *name = key.CStr();

    if (!strcmp(name, "radHearShot")) { st->perc.senses.radHearShot = val; return true; }
    if (!strcmp(name, "radHearRun"))  { st->perc.senses.radHearRun  = val; return true; }
    if (!strcmp(name, "radHearWalk")) { st->perc.senses.radHearWalk = val; return true; }
    if (!strcmp(name, "isYieldWay")) {
        if (val <= 0.5f)
            st->mind.val &= ~0x10000000;
        else
            st->mind.val |= 0x10000000;
        return true;
    }
    return false;
}
