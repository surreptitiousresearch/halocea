#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/fsm/fsmAIMING_EVENT.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// aiBRAIN::implEndInterest @ 0x8329A550
// ?implEndInterest@aiBRAIN@@UAAXXZ
//
// SSL op: stop the scripted look-at. Rejected by the SSL common back-off gate; otherwise send the
// body FSM the "off_look_at" event and clear the scripted-interest body state bit (0x8000000). Logs
// an error when there is no body FSM.
void aiBRAIN::implEndInterest()
{
    dsTSTRING<char> callName("EndInterest()");
    if (IsSSLCommonBackOff(callName, dsFLAGS<SSL_COMMON_BACKOFF, int>{0}))
        return;

    aiBODY_IFACE *bodyIface = body.pointee;
    if (bodyIface->SendFsmEventByName_2(&fsmAIMING_EVENT::off_look_at_()))
        st.pointee->body.val &= ~0x8000000u;
    else
        _apLog("~AI,Ssl,Error~'%s' can't call EndInterest(): AI hasn't body FSM", names.nameInst);
}
