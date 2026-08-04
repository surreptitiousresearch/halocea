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

// aiBRAIN::OnLeaveHO() @ 0x83255A30
// ?OnLeaveHO@aiBRAIN@@QAAXXZ
//
// Leave hideout mode: the inverse of OnEnterHO. Unless the common SSL back-off gate rejects the
// "OnLeaveHO()" call, and only when the "in-HO event fired" status flag IS set, fire the descriptor
// "OnLeaveHO" SSL event on this brain's script object and clear the status flag.
void aiBRAIN::OnLeaveHO()
{
    dsTSTRING<char> callName;
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (!IsSSLCommonBackOff(*dsSPrintf(&callName, "OnLeaveHO()"), noFlags)
        && (st.pointee->gen.val & 0x1000000) != 0)
    {
        sslObject.HandleEvent(aiBRAIN_DESC_SslFnOnLeaveHO(spDesc.pointee));
        st.pointee->gen.val &= ~0x1000000;
    }
}
