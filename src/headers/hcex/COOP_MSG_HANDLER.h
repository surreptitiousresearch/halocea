#pragma once
/* COOP_MSG_HANDLER — the HCEX co-op networking bridge: an mp::MESSAGE_HANDLER-derived object
 * (the singleton `coopMsgHandler`) subscribed to the raw-input and deterministic-input relay
 * messages. It queues the remote peer's deterministic action envelopes (detInput) and remembers
 * the last raw-input packet (lastRawInput). DB-verified layout (types_members COOP_MSG_HANDLER):
 * mp::MESSAGE_HANDLER base @0 (4B), detInput @4 (dsDEQUE<haloPLAYER_ACTIONS_ENV>, 32B),
 * lastRawInput @36 (haloRAW_INPUT_X360_ENV, 24B) — size 60. */

#include "../ws/ds/dsDEQUE.h"
#include "haloPLAYER_ACTIONS_ENV.h"
#include "haloRAW_INPUT_X360.h"

namespace mp {
// mp::MESSAGE_HANDLER — relay message receiver base. DB: 4 bytes (a single vtable pointer).
// Boundary: only used here as the zero-offset base of COOP_MSG_HANDLER.
struct MESSAGE_HANDLER { void *__vftable; /* 0x00 */ };
}

/* haloRAW_INPUT_X360_ENV — a raw-input packet tagged with its message id / tick / desync-CRC.
 * DB-verified: haloRAW_INPUT_X360 base @0 (12B), msgId@12, tick@16, crc@20 — size 24. */
struct haloRAW_INPUT_X360_ENV : haloRAW_INPUT_X360 {
    int msgId; // 0x0C monotonically increasing send id
    int tick;  // 0x10 game tick this input applies to
    int crc;   // 0x14 sender's game-state CRC at that tick
};

typedef struct COOP_MSG_HANDLER : mp::MESSAGE_HANDLER {
    dsDEQUE<haloPLAYER_ACTIONS_ENV> detInput;     // 0x04 queued remote deterministic actions
    haloRAW_INPUT_X360_ENV          lastRawInput; // 0x24 most recent raw-input packet received
} COOP_MSG_HANDLER;

extern COOP_MSG_HANDLER coopMsgHandler;
