#pragma once
#include "../ds/dsVECTOR.h"
#include "saLIST.h"
// ws-engine msg: the global message-subscription registry. Holds a vector of saLIST entries,
// kept sorted by message id, each mapping a message id to the addresses subscribed to it.
// DB-verified layout (types_members msgSUBSCRIBER): __vftable@0, msgPairList@4 — size 24.

struct msgADDR;
struct msgSUBSCRIBER_vtbl;

typedef struct msgSUBSCRIBER {
    msgSUBSCRIBER_vtbl  *__vftable;   // 0x00
    dsVECTOR<saLIST, 8>  msgPairList; // 0x04 msg-id -> subscriber-list table (sorted by msg id)

    // 0x82AC6718 — broadcast `msg` to every address subscribed to it.
    void SendMsg(int msg, void *pInfo, msgADDR *pSender);
    // 0x82AC7078 (?RegisterPair@msgSUBSCRIBER@@QAAHPAVmsgADDR@@H@Z) — subscribe `pAddr` to `msg`
    // (creating the (msg -> list) entry if absent). Returns the resulting entry index.
    int RegisterPair(msgADDR *pAddr, int msg);
    // 0x82AC65F8 — unsubscribe `pAddr` from `msg`; drops the whole entry when its list empties.
    void UnRegisterPair(msgADDR *pAddr, int msg);
    // 0x82AC6928 — unsubscribe `pAddr` from every message id, dropping any (msg -> list) entry that
    // becomes empty.
    void RemoveSubscriber(msgADDR *pAddr);
} msgSUBSCRIBER;

typedef struct msgSUBSCRIBER_vtbl {
    void (*dtr_msgSUBSCRIBER)(msgSUBSCRIBER *self); // 0x00
} msgSUBSCRIBER_vtbl;

// ?_msgSubscr — the singleton subscription registry the message system dispatches through.
extern msgSUBSCRIBER _msgSubscr;
