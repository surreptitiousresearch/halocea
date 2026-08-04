#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// aiBRAIN::OnEnterHO() @ 0x83255950
// ?OnEnterHO@aiBRAIN@@QAAXXZ
//
// Enter hideout mode. Unless the common SSL back-off gate rejects the "OnEnterHO()" call, and only
// when the "in-HO event fired" status flag is not already set, fire the descriptor "OnEnterHO" SSL
// event on this brain's script object and set the status flag.
void aiBRAIN::OnEnterHO()
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    // st.pointee->gen bit 0x1000000: "OnEnterHO event already fired" (AI_GEST flag — enum name
    // absent from the DB; kept as the raw bit the disassembly tests/sets).
    if (!IsSSLCommonBackOff(*dsSPrintf(&callName, "OnEnterHO()"), noFlags)
        && (st.pointee->gen.val & 0x1000000) == 0)
    {
        sslObject.HandleEvent(aiBRAIN_DESC_SslFnOnEnterHO(spDesc.pointee));
        st.pointee->gen.val |= 0x1000000;
    }
}
