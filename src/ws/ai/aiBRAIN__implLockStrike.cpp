// aiBRAIN::implLockStrike @ 0x832B2FA0
// ?implLockStrike@aiBRAIN@@UAAXABV?$dsTSTRING@D@@M@Z  (virtual)
//
// SSL "LockStrike(name, time)" body: unless the common SSL back-off gate rejects the call, lock the
// named strike for `time` seconds on the brain's active weapon module, logging when no such strike
// exists.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

void aiBRAIN::implLockStrike(const dsTSTRING<char> &nameStrike, float time)
{
    dsTSTRING<char> callName;
    if (this->IsSSLCommonBackOff(*dsSPrintf(&callName, "LockStrike()"),
                                 dsFLAGS<SSL_COMMON_BACKOFF, int>{0}))
        return;

    aiWEAPON_BASE *wpn = this->body.pointee->wpn.pointee;
    if (wpn && !wpn->LockStrike(nameStrike, time))
        _apLog("~AI,Weapon~'%s': can't lock strike '%s': strike not found",
               this->names.nameInst, nameStrike.pBuffer->str);
}
