#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/dsVECTOR.h"
#include "../ds/PTR_LIST.h"
#include "../ds/MAP.h"
#include "msgQUEUE.h"
#include "msgTIME_QUEUE.h"
#include "msgADDR.h"
#include "msgDSP_RES.h"
// ws-engine msg: the central actor message-passing system. Tracks all live msgADDR endpoints
// (in priority buckets and a uid hash), owns the per-frame and time-delayed message queues, and
// dispatches messages to recipients.
// DB-verified layout (types_members msgSYSTEM):
//   stateSys@0, addrList1@4 (20), addrHash@24 (84), msgQueue@108 (44), timeQueue@152 (20),
//   listChangePrior@172 (20), frameNmb@192, addrDeleted@196 (20) — size 216.

typedef struct msgSYSTEM {
    // Iterates one priority bucket (ind1 selects the bucket in addrList1) plus the intrusive
    // msgADDR position within it. DB-verified layout (types_members msgSYSTEM::ITERATOR):
    // ind1@0 (int), it@4 (ds::PTR_LIST<msgADDR,...>::ITERATOR) -- size 8.
    struct ITERATOR {
        int ind1; // 0x00 bucket index into addrList1
        ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR it; // 0x04

        // 0x82555B60 — construct pointing at bucket `ind1`, position `pAddr` within it.
        ITERATOR(int ind1, ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR it);

        // Un-reversed siblings (not in this batch's scope) -- boundaries.
        ITERATOR(); // 0x82557F20 // boundary
        bool IsDone() const; // 0x825084E8 // boundary
        msgADDR *Get() const; // 0x8250B450 // boundary
        msgADDR *operator->() const; // 0x8250B438 // boundary
        ITERATOR &operator++(); // 0x8255A0E0 // boundary
    };

    apSTATE_T<unsigned long> stateSys; // 0x00 system state bitmask

    // 0x04 per-priority buckets: a vector of intrusive address lists indexed by priority band.
    dsVECTOR<ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >, 8> addrList1;

    // 0x18 uniqueID -> msgADDR* index. Fully reversed (msgADDR-map batch): see MAP.h and the
    // MAP_int_msgADDRptr__*.cpp / dsNODE_CACHE_*DUMMY__*.cpp / PTR_LIST_dsMsgAddrMapDUMMY__*.cpp
    // method bodies.
    ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> addrHash; // 0x18 (84B)

    msgQUEUE      msgQueue;     // 0x6C per-frame message queue (double-buffered)
    msgTIME_QUEUE timeQueue;    // 0x98 time-delayed message queue

    // 0xAC addresses awaiting a priority re-sort (kept sorted by priorityInt).
    dsVECTOR<msgADDR *, 8> listChangePrior;

    unsigned int  frameNmb;    // 0xC0 current frame number

    // 0xC4 addresses unregistered this frame, deferred for teardown.
    dsVECTOR<msgADDR *, 8> addrDeleted;

    // Return an iterator positioned at the first registered actor (highest-priority bucket).
    // Returned BY VALUE (sret). Used to seed msgTYPE_ITER<T> traversals.  boundary.
    ITERATOR Front() const;

    // 0x82555E38 (?AddSubscriber@msgSYSTEM@@QAAHPAVmsgADDR@@H@Z) — subscribe `pSubscr` to `msg`
    // (lifecycle events 3/4 etc.). Returns nonzero on success. boundary.
    int AddSubscriber(msgADDR *pSubscr, int msg);

    // 0x82555ED8 — forward `msg` to the subscription registry so subscribers are notified.
    void NotifySubscribers(int msg, void *pInfo, msgADDR *pSender);
    // 0x82555E78 — unsubscribe `pSubscr` from `msg` (msg<=0 unsubscribes from everything).
    void RemoveSubscriber(msgADDR *pSubscr, int msg);
    // 0x82557370 (?PostTimeMsg@msgSYSTEM@@QAAXPAVmsgADDR@@HM0@Z) — queue `msg` for delivery to
    // `pRecv` after `time` seconds, attributed to sender `pSend`. boundary.
    void PostTimeMsg(msgADDR *pRecv, int msg, float time, msgADDR *pSend);
    // 0x82557178 — deliver `msg` to `pRecv` if it is registered and either accepts general
    // messages or `msg` is a lifecycle event (2/4).
    msgDSP_RES SendMsg(msgADDR *pRecv, int msg, void *pInfo, msgADDR *pSend);
    // 0x82559EF0 — purge `pAddr` from the change-priority list and both message queues.
    void RemoveFromQueues(msgADDR *pAddr);
    // 0x82557238 — queue `msg` for delivery to `pRecv` next frame (only if it accepts messages);
    // a terminate message (2) also sets the term-posted bit and notifies the recipient.
    void PostMsg(msgADDR *pRecv, int msg, msgADDR *pSend);
    // 0x8255D4F8 — register `pAddr`'s uniqueID in the uid hash (idempotent; asserts an existing
    // entry maps back to `pAddr`).
    void AssignUID(msgADDR *pAddr);
    // 0x82556C88 (?FindUID@msgSYSTEM@@QAAPAVmsgADDR@@K@Z) — look up the registered actor with
    // uniqueID `uid` in the uid hash; null if none. boundary.
    msgADDR *FindUID(unsigned int uid);

    // msgADDR::Register drives registration through the private InsertAddr helper.
    friend struct msgADDR;

private:
    // 0x8255D680 — link `pAddr` into its priority bucket (creating the bucket if this priority is
    // new), mark it priority-managed, and assign its uid.
    void InsertAddr(msgADDR *pAddr);
    // 0x82556F60 — run `msg` through `pRecv`'s pre/process/post dispatch chain, honouring its
    // run/suspend/terminate state; returns whether the message was sent, filtered out, or errored.
    msgDSP_RES Dispatch(msgADDR *pRecv, int msg, void *pInfo, msgADDR *pSend);
} msgSYSTEM;

// ?msgSystem — the singleton message system.
extern msgSYSTEM msgSystem;
