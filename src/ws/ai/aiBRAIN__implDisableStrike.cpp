// aiBRAIN::implDisableStrike @ 0x832B2EC8
// ?implDisableStrike@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z  (virtual)
//
// SSL "DisableStrike(name)" body: unless the common SSL back-off gate rejects the call, disable the
// named strike on the brain's active weapon module (EnableStrike(..., false)), logging when no such
// strike exists.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

void aiBRAIN::implDisableStrike(const dsTSTRING<char> &nameStrike)
{
    dsTSTRING<char> callName;
    if (this->IsSSLCommonBackOff(*dsSPrintf(&callName, "DisableStrike()"),
                                 dsFLAGS<SSL_COMMON_BACKOFF, int>{0}))
        return;

    aiWEAPON_BASE *wpn = this->body.pointee->wpn.pointee;
    if (wpn && !wpn->EnableStrike(nameStrike, false))
        _apLog("~AI,Weapon~'%s': can't disable strike '%s': strike not found",
               this->names.nameInst, nameStrike.pBuffer->str);
}
