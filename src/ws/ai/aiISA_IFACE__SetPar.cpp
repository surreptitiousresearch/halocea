#include "../../headers/ws/ai/aiISA_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

// aiISA_IFACE::SetPar() @ 0x8329FDD8
// ?SetPar@aiISA_IFACE@@UAA_NABV?$dsTSTRING@D@@M@Z
//
// Virtual: apply the shared "viewPercAlert" tunable to the status block's perception snapshot.
// Enables manual view-percent only when the value is in the sensible [0,100] range.
bool aiISA_IFACE::SetPar(const dsTSTRING<char> &key, float val)
{
    if (strcmp(key.CStr(), "viewPercAlert") != 0)
        return false;

    aiSTATUS *st = pBrain->st.pointee;
    st->perc.viewPercForAlert = val;
    st->perc.shouldUseManualViewPercent = (val >= 0.0f && val <= 100.0f);
    return true;
}
