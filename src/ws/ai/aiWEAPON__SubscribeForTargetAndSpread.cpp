#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/fsm/fsmCB_HANDLER.h"

// aiWEAPON::SubscribeForTargetAndSpread @ 0x832967F0
// ?SubscribeForTargetAndSpread@aiWEAPON@@UAA_NPAVpropWPN_AI_USABLE@@@Z
//
// Hook this weapon module's target/spread responders onto the controller's equipped weapon FSM: for
// both the "request target" and "request spread" events, register the matching OnXxxRequested member
// on the weapon's event handler (which lives at offset 0x3C inside the wpnWEAPON). Returns false when
// there is no controller. The two RegisterCB registration handles are intentionally discarded.
bool aiWEAPON::SubscribeForTargetAndSpread(propWPN_AI_USABLE *ctrl)
{
    if (!ctrl)
        return false;

    // The weapon's fsmCB_HANDLER<dsSTRID> is embedded at wpnWEAPON+0x3C (matches the disasm
    // `addi r4, weapon, 0x3C`).
    fsmCB_HANDLER<dsSTRID> *tgtHandler =
        reinterpret_cast<fsmCB_HANDLER<dsSTRID> *>(reinterpret_cast<char *>(ctrl->weapon()) + 0x3C);
    tgtHandler->RegisterCB<aiWEAPON, fsmWPN_TGT_INFO>(
        fsmWEAPON_EVENT::request_tgt_(), this, &aiWEAPON::OnTgtRequested);

    fsmCB_HANDLER<dsSTRID> *sprdHandler =
        reinterpret_cast<fsmCB_HANDLER<dsSTRID> *>(reinterpret_cast<char *>(ctrl->weapon()) + 0x3C);
    sprdHandler->RegisterCB<aiWEAPON, fsmWPN_SPRD_INFO>(
        fsmWEAPON_EVENT::request_sprd_(), this, &aiWEAPON::OnSpreadRequested);

    return true;
}
