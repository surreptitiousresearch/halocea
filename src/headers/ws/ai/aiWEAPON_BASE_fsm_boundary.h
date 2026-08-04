#pragma once
#include "../ds/dsSTRID.h"
#include "aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmCB_HANDLER_dsSTRID (eventHandler @0x3C)
// ws-engine ai08/fsm boundary — the weapon-controller FSM event ids and the propFSM cast reached by
// aiWEAPON_BASE::(Un)SubscribeForTargetAndSpread. A propWPN_AI_USABLE's wpnWEAPON is a propFSM
// (its event handler lives at propFSM::eventHandler); the subscribe/unsubscribe pair (de)registers
// this weapon module against the weapon FSM's request events. boundary — bodies external to batch.

// fsmWEAPON_EVENT static event-id accessors (real statics returning interned dsSTRID refs).
namespace fsmWEAPON_EVENT {
    // ?request_tgt_@fsmWEAPON_EVENT@@SAABVdsSTRID@@XZ @ 0x829930F8
    const dsSTRID &request_tgt_();
    // ?request_sprd_@fsmWEAPON_EVENT@@SAABVdsSTRID@@XZ @ 0x82F3AC28
    const dsSTRID &request_sprd_();
}
