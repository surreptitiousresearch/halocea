#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// aiBRAIN::OnReload() @ 0x832B31A8
// ?OnReload@aiBRAIN@@QAAXXZ
//
// The brain's weapon just reloaded: unless the common SSL back-off gate rejects the "OnReload()"
// call, fire the descriptor "OnReload" no-argument SSL event on this brain's script object.
void aiBRAIN::OnReload()
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (!IsSSLCommonBackOff(*dsSPrintf(&callName, "OnReload()"), noFlags))
    {
        // RAII: the returned sslERROR temporary releases itself at statement end.
        sslObject.HandleEvent(aiBRAIN_DESC_SslEvOnReload(spDesc.pointee));
    }
}
