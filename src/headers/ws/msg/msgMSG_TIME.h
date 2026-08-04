#pragma once
#include "msgMSG.h"
#include "../ds/PTR_LIST_NODE.h"
// ws-engine msg: a delayed message queued in an msgTIME_QUEUE. Extends msgMSG with an intrusive
// list node. DB-verified layout (types_members msgMSG_TIME): msgMSG@0 (24), PTR_LIST_NODE@24 (8)
// — size 32. The leading msgMSG base member is modeled as public inheritance.

struct msgMSG_TIME : msgMSG {
    ds::PTR_LIST_NODE listNode; // 0x18 hook in the time queue's pending list
};
