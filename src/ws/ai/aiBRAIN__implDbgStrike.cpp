#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// aiBRAIN::implDbgStrike() @ 0x832B3080
// ?implDbgStrike@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z
//
// SSL debug op: unless the common SSL back-off gate rejects the "DbgStrike()" call, leave only the
// strike named `nameStrike` enabled on the body's weapon module (when the body currently has one).
void aiBRAIN::implDbgStrike(const dsTSTRING<char> &nameStrike)
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (IsSSLCommonBackOff(*dsSPrintf(&callName, "DbgStrike()"), noFlags))
        return;

    aiWEAPON_BASE *wpn = body.pointee->wpn.pointee;
    if (wpn)
        wpn->DbgLeaveOneEnabled(nameStrike);
}
