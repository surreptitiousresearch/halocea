#pragma once
/* Shared boundary declarations for the HCEX co-op input path (coop_input_initialize @0x823BB198
 * and main_coop_update @0x823C0DB8). These wire the co-op networking bridge (coopMsgHandler) to
 * the relay message dispatcher and drive the per-tick raw-input / deterministic-input exchange
 * between the two peers. The ws-engine mp* / net* messaging surface and the Blam-bridge helpers
 * stay externs. */

#include "COOP_MSG_HANDLER.h"                 // coopMsgHandler, haloRAW_INPUT_X360_ENV
#include "haloENGINE_CONTROL.h"               // haloENGINE_CONTROL, haloEngineCtrl, Get/SetRawInput, GetDetAction
#include "haloPLAYER_ACTIONS_ENV.h"
#include "../ws/net/SESSION_BASE.h"           // SESSION_BASE
#include "../ws/net/RELAY.h"                  // net::RELAY, net::CLIENT_ID
#include "../ws/ds/dsDATA.h"                  // dsDATA

// --- ws-engine mp (multiplayer messaging) surface -------------------------------------------
namespace net {
    // Bitmask of relay client slots a message is addressed to (DB: 4-byte flag set).
    enum CLIENT_MASK { CLIENT_MASK_NONE = 0 };
}

namespace mp {
    // Relay message id (DB enum mp::DISPATCHER_MSGID). Only the two co-op inputs are named here.
    enum DISPATCHER_MSGID { MSG_RAW_INPUT = 0, MSG_DET_INPUT = 1 };

    // mp::MESSAGE_DISPATCHER — routes relay messages to subscribed handlers. Boundary; only the
    // two entry points the co-op path uses are declared.
    struct MESSAGE_DISPATCHER {
        // 0x... — register `handler` for `msg`; `clientFilter` == -1 means "any sender".
        void SubscribeMessage(DISPATCHER_MSGID msg, MESSAGE_HANDLER *handler, int clientFilter);
        // 0x... — send `data` for `msg` to the clients named by `mask`.
        void SendMessage(net::CLIENT_MASK mask, DISPATCHER_MSGID msg, dsDATA *data);
    };
}

// dsDATA::Construct<T> — construct an empty typed value of T into `data` and return a pointer to
// its storage (installs the dsDATA_TYPE_STORAGE<T> descriptor). Static template, boundary.
template<class T> T *dsDATA_Construct(dsDATA *data);

// --- co-op module-scope globals (HCEX halo_coop.cpp) ----------------------------------------
extern "C" SESSION_BASE *pSyncSession;            // active co-op session (null == not in co-op)
extern "C" int           msgId;                    // outgoing raw-input message sequence counter
extern int           hcex_data_applied_num;    // count of deterministic ticks applied so far
extern "C" int           hcex_coop_nticks_to_apply;// pending queued ticks still to be applied
extern int           hcex_num_update;          // co-op update invocation counter
extern haloENGINE_CONTROL *haloEngineCtrl;      // the Blam engine-control bridge singleton

// --- Blam-bridge helpers (boundary) ---------------------------------------------------------
extern "C" {
    int  game_state_calculate_crc(void);
    void hcex_collect_crc(void);
    void hcex_check_crc(void);
    void hcex_coop_process_raw_input(const haloRAW_INPUT_X360 *local_input,
                                     const haloRAW_INPUT_X360 *remote_input,
                                     haloPLAYER_ACTIONS_ENV *out_actions);
    void hcex_coop_process_deteministic_data(haloPLAYER_ACTIONS_ENV *env);
}
