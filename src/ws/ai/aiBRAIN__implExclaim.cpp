#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf @ 0x825210C0 — formatted build into a new dsTSTRING (returned by value). boundary.
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);

// aiBRAIN::implExclaim @ 0x832994B0
// ?implExclaim@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z
//
// Fire the brain's "Exclaim()" SSL common back-off check. The passed `exName` is not consulted by
// this implementation — the back-off host resolves the current exclamation itself.
void aiBRAIN::implExclaim(const dsTSTRING<char> &exName)
{
    (void)exName;
    IsSSLCommonBackOff(dsSPrintf("Exclaim()"), dsFLAGS<SSL_COMMON_BACKOFF, int>());
}
