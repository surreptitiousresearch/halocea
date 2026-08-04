#pragma once
#include "../ds/PTR_LIST.h"
#include "msgMSG_TIME.h"
// ws-engine msg: time-ordered queue of delayed messages. DB-verified layout (types_members
// msgTIME_QUEUE): nMsgReady@0, msgList@4 (12-byte ds::PTR_LIST), itNextMsg@16 — size 20.

struct msgADDR;
struct msgMSG;

typedef struct msgTIME_QUEUE {
    int nMsgReady; // 0x00 count of messages whose delay has elapsed and are ready to fire
    ds::PTR_LIST<msgMSG_TIME, ds::LIST_NODE_ACCESS_POLICY<msgMSG_TIME> >           msgList;   // 0x04
    ds::PTR_LIST<msgMSG_TIME, ds::LIST_NODE_ACCESS_POLICY<msgMSG_TIME> >::ITERATOR itNextMsg; // 0x10

    // 0x82AC3A68 — tear down a dequeued delayed message: release its payload, delete it, null the slot.
    void FreeMsg(msgMSG_TIME **ppMsg);
    // 0x82AC4560 — walk the pending list and drop every delayed message whose recipient is `pAddr`
    // (adjusting the ready count and the dispatch cursor), and null the sender on messages sent by it.
    void DeleteMsgByAddr(msgADDR *pAddr);
} msgTIME_QUEUE;
