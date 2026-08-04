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

// aiBRAIN::OnStrikeStart() @ 0x832B3360
// ?OnStrikeStart@aiBRAIN@@QAAXABVdsSTRID@@@Z
//
// The named strike began firing: unless the common SSL back-off gate rejects the "OnStrikeStart()"
// call, box the strike name into a dsDATA event value and fire the descriptor "OnStrikeStart" SSL
// event on this brain's script object.
void aiBRAIN::OnStrikeStart(const dsSTRID &name)
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (IsSSLCommonBackOff(*dsSPrintf(&callName, "OnStrikeStart()"), noFlags))
        return;

    // Box the strike name into a dsDATA event value (StoreValue makes its own ref-counted copy).
    dsTSTRING<char> arg;
    arg.UnsafeInit(name.CStr(), -1, 0);
    dsDATA event;
    event.type = nullptr;
    event.StoreValue<dsTSTRING<char> >(arg);

    // RAII: the returned sslERROR temporary and `event`/`arg` release themselves at scope end.
    sslObject.HandleEvent(aiBRAIN_DESC_SslEvOnStrikeStart(spDesc.pointee), event);
}
