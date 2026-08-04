#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiISA_BASE.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

// aiBRAIN::OnAlert() @ 0x832A0A90
// ?OnAlert@aiBRAIN@@UAAHW4AI_ALERT@@@Z
//
// Virtual: an alert of kind `flag` fired. Unless the common SSL back-off gate rejects it or the ISA
// mind module is already zeroed, notify the body of the alert position and dispatch the descriptor
// "OnAlert" SSL event with the alert-kind name as its boxed payload.
int aiBRAIN::OnAlert(AI_ALERT flag)
{
    dsTSTRING<char> callName;
    callName.UnsafeInit("OnAlert()", -1, 0);
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (IsSSLCommonBackOff(callName, noFlags))
        return 1;

    if (mind.isa.pointee->IsZero())
    {
        _apLog("~AI,Senses~'%s': SSL call OnAlert rejected: ISA system is already zero.", names.nameInst);
        return 1;
    }

    body.pointee->NoticeAlert(&cmd.posAlert);

    const char *alertName;
    switch (flag)
    {
    case AI_ALERT_SEE:
        alertName = "SEE";
        _apLog("~AI,Senses~'%s': SSL call OnAlert(\"SEE\") is coming...", names.nameInst);
        break;
    case AI_ALERT_HEAR:
        alertName = "HEAR";
        _apLog("~AI,Senses~'%s': SSL call OnAlert(\"HEAR\") is coming...", names.nameInst);
        break;
    case AI_ALERT_DAMAGE:
        alertName = "DAMAGE";
        _apLog("~AI,Senses~'%s': SSL call OnAlert(\"DAMAGE\") is coming...", names.nameInst);
        break;
    case AI_ALERT_PHYS_OBJ:
        alertName = "PHYS_OBJ";
        _apLog("~AI,Senses~'%s': SSL call OnAlert(\"PHYS_OBJ\") is coming...", names.nameInst);
        break;
    case AI_ALERT_INTEROP:
        alertName = "INTEROP";
        _apLog("~AI,Senses~'%s': SSL call OnAlert(\"INTEROP\") is coming...", names.nameInst);
        break;
    case AI_ALERT_FROM_SCRIPT:
        alertName = "SCRIPT";
        _apLog("~AI,Senses~'%s': SSL call OnAlert() is coming...", names.nameInst);
        break;
    default:
        return 0;
    }

    // Box the alert-kind name into a dsDATA event value (StoreValue makes its own ref-counted copy).
    dsTSTRING<char> arg;
    arg.UnsafeInit(alertName, -1, 0);
    dsDATA event;
    event.type = nullptr;
    event.StoreValue<dsTSTRING<char> >(arg);

    // RAII: the returned sslERROR temporary and `event`/`arg` release themselves at scope end.
    sslObject.HandleEvent(aiBRAIN_DESC_SslEvOnAlert(spDesc.pointee), event);
    return 1;
}
