#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// aiBRAIN::OnStrikeEnd() @ 0x832B34C0
// ?OnStrikeEnd@aiBRAIN@@QAAXABVdsSTRID@@@Z
//
// The named strike finished firing: unless the common SSL back-off gate rejects the "OnStrikeEnd()"
// call, box the strike name into a dsDATA event value and fire the descriptor "OnStrikeEnd" SSL
// event on this brain's script object.
void aiBRAIN::OnStrikeEnd(const dsSTRID &name)
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (IsSSLCommonBackOff(*dsSPrintf(&callName, "OnStrikeEnd()"), noFlags))
        return;

    // Box the strike name into a dsDATA event value (StoreValue makes its own ref-counted copy).
    dsTSTRING<char> arg;
    arg.UnsafeInit(name.CStr(), -1, 0);
    dsDATA event;
    event.type = nullptr;
    event.StoreValue<dsTSTRING<char> >(arg);

    // RAII: the returned sslERROR temporary and `event`/`arg` release themselves at scope end.
    sslObject.HandleEvent(aiBRAIN_DESC_SslEvOnStrikeEnd(spDesc.pointee), event);
}
