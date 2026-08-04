#pragma once
#include "../ds/dsVECTOR.h"
#include "msgMSG.h"
// ws-engine msg: double-buffered frame message queue. Messages posted this frame land in
// nextQueue and are drained from currQueue. DB-verified layout (types_members msgQUEUE):
//   currQueue@0 (20), nextQueue@20 (20), nextMsg@40 — size 44.

struct msgADDR;
struct msgDATA;

typedef struct msgQUEUE {
    dsVECTOR<msgMSG, 8> currQueue; // 0x00 messages being processed this frame
    dsVECTOR<msgMSG, 8> nextQueue; // 0x14 messages posted for next frame
    int                 nextMsg;   // 0x28 index of the next message to dispatch in currQueue

    // 0x82AC5770 — append an envelope for `msg` (with an optional deep-copied `pInfo` payload) to
    // next frame's queue, addressed from `pSend` to `pRecv`.
    void PutMsg(msgADDR *pRecv, int msg, msgDATA *pInfo, msgADDR *pSend);
    // 0x82AC4EA0 — drop every queued message (in both buffers) whose recipient/sender is `pAddr`
    // by nulling the matching pRecv/pSend slots.
    void DeleteMsgByAddr(msgADDR *pAddr);
} msgQUEUE;
