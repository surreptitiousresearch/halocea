#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// The ai08 planner singleton (aiEXIFACE.h). Declared locally to avoid its header's conflicting
// gsTEAMS boundary definition.
extern aiPLANNER *aiPlanner;
// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// aiBRAIN::implSetNavSys(const dsTSTRING<char>&) @ 0x83247C30
// ?implSetNavSys@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z  (virtual)
// SSL op: request this brain be localized to the nav-system named `name`. If the SSL common
// back-off gate rejects the "SetNavSys()" call, do nothing; otherwise hand the request to the
// planner.
void aiBRAIN::implSetNavSys(const dsTSTRING<char> &name)
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (!IsSSLCommonBackOff(*dsSPrintf(&callName, "SetNavSys()"), noFlags))
        aiPlanner->SetNavSystemToUnit(this, name);
}
