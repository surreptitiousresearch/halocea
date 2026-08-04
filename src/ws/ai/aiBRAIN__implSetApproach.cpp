#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiAPPROACH_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// aiBRAIN::implSetApproach(const dsTSTRING<char> &) @ 0x8329A800
// ?implSetApproach@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z
//
// Virtual "SetApproach" implementation: unless the common SSL back-off gate rejects the
// "SetApproach()" call, enable the brain's approach ("aprog") module and load the named section.
void aiBRAIN::implSetApproach(const dsTSTRING<char> &name)
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (!IsSSLCommonBackOff(*dsSPrintf(&callName, "SetApproach()"), noFlags))
    {
        mind.aprog.pointee->Enable(true);
        mind.aprog.pointee->SetByName(name);
    }
}
