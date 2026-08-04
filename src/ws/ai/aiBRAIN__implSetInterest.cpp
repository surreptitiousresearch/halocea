#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/fsm/fsmAIMING_EVENT.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// aiBRAIN::implSetInterest @ 0x8329A3D0
// ?implSetInterest@aiBRAIN@@UAAXABV?$dsTSTRING@D@@0_N@Z
//
// SSL op: make the AI look at `interestPoint` using look-at mode `interestMode`. Rejected by the SSL
// common back-off gate; otherwise send the body FSM a "look_at" event. When the FSM accepts, set the
// scripted-interest body state bit (0x8000000); if there is no body FSM, log an error.
void aiBRAIN::implSetInterest(const dsTSTRING<char> &interestPoint,
                              const dsTSTRING<char> &interestMode,
                              bool disableIdling)
{
    dsTSTRING<char> callName("SetInterest()");
    if (IsSSLCommonBackOff(callName, dsFLAGS<SSL_COMMON_BACKOFF, int>{0}))
        return;

    fsmAIMING_EVENT::LOOK_AT_PARAM param;
    param.mode = interestMode;
    param.trk = interestPoint;
    param.isAccepted = false;
    param.isIdlingDisabled = disableIdling;

    aiBODY_IFACE *bodyIface = body.pointee;
    if (bodyIface->SendFsmEventByName(&fsmAIMING_EVENT::look_at_(), &param) && param.isAccepted)
    {
        st.pointee->body.val |= 0x8000000u;
    }
    else
    {
        _apLog("~AI,Ssl,Error~'%s' can't call SetInterest(): AI hasn't body FSM", names.nameInst);
    }
}
