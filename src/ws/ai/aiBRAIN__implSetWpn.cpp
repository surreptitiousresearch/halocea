#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// aiBRAIN::implSetWpn() @ 0x832B3130
// ?implSetWpn@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z
//
// SSL op: run only the common SSL back-off gate for the "SetWpn()" call (which logs / rejects the
// script call). The base implementation performs no weapon change — `wpnName` is unused and the
// gate result is discarded; concrete species override this to actually swap weapons.
void aiBRAIN::implSetWpn(const dsTSTRING<char> &wpnName)
{
    (void)wpnName;
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    IsSSLCommonBackOff(*dsSPrintf(&callName, "SetWpn()"), noFlags);
}
