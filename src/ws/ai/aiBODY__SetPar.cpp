#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include <string.h>

// 0x832AC2A0 ?SetPar@aiBODY@@UAA_N...M@Z
// Apply a named force-aim tunable to the status body-flags; "isSendNoAimEvent" and unknown keys are
// forwarded to the weapon module. `val > 0.5` is the boolean-on threshold.
//   isForceAim       -> bit 0x200000 (and clears 0x400000 when on)
//   isForceNoAim     -> bit 0x400000 (and clears 0x200000 when on)
//   isSendNoAimEvent -> bit 0x10000  (and clears 0x20000 when the event bit ends up set)
bool aiBODY::SetPar(const dsTSTRING<char> &key, float val)
{
    const char *k = key.CStr();

    if (strcmp(k, "isForceAim") == 0)
    {
        aiSTATUS *status = this->pBrain->st.pointee;
        unsigned int v = (unsigned int)status->body.val;
        status->body.val = (int)((val > 0.5f) ? (v | 0x200000u) : (v & 0xFFDFFFFFu));
        if (val > 0.5f)
            this->pBrain->st.pointee->body.val &= ~0x400000;
        return true;
    }

    if (strcmp(k, "isForceNoAim") == 0)
    {
        aiSTATUS *status = this->pBrain->st.pointee;
        unsigned int v = (unsigned int)status->body.val;
        status->body.val = (int)((val > 0.5f) ? (v | 0x400000u) : (v & 0xFFBFFFFFu));
        if (val > 0.5f)
            this->pBrain->st.pointee->body.val &= ~0x200000;
        return true;
    }

    if (strcmp(k, "isSendNoAimEvent") == 0)
    {
        aiSTATUS *status = this->pBrain->st.pointee;
        unsigned int v = (unsigned int)status->body.val;
        status->body.val = (int)((val > 0.5f) ? (v | 0x10000u) : (v & 0xFFFEFFFFu));
        if ((status->body.val & 0x10000) != 0)
            status->body.val &= ~0x20000;
        return true;
    }

    // Unknown key: forward to the weapon module (key + value both passed through).
    return this->wpn.pointee->SetPar(key, val);
}
