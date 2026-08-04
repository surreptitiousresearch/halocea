#pragma once
#include "dsSTRID.h"
#include "dsPARAM_LIST.h"
#include "dsVECTOR_PARAM_LIST.h"
#include "dsLF_SORTED_MAP.h"
#include "dsPOOL.h"
#include "dsSTR_HASH.h"
#include "../os/osLOCK.h"
#include "dsEVENT_HANDLER.h"
#include "MAP.h"
// ws-engine ds: event manager. DB-verified top-level layout (types_members dsEVENT_MGR):
//   lock@0 (osLOCK,52), itemMap@52 (dsLF_SORTED_MAP<char const*,int,...>,36),
//   items@88 (dsPOOL<ITEM_REC,256,256>,1040), handlers@1128 (dsPOOL<ITEM_HANDLER,512,256>,1040),
//   events@2168 (dsPOOL<EVENT,1024,512>,2064), subscriberMap@4232 (ds::MAP<void const*,int,...>,84),
//   subscribers@4316 (dsPOOL<SUBSCRIBER_REC,64,64>,272), threadInfo@4588 (THREAD_INFO[8],2656).

struct dsSTR_CMP;       // boundary — const char* ordering policy for the item map

typedef struct dsEVENT_MGR {
    // DB-verified (types_members dsEVENT_MGR::EVENT): id@0, params@4 (dsVECTOR_PARAM_LIST),
    // idxNext@32, refCounters@36 (int[8]), age@68, threadMask@72 — size 76.
    // While free, `id` overlays the pool's "next free index" link.
    struct EVENT {
        int                 id;             // 0x00
        dsVECTOR_PARAM_LIST params;         // 0x04 named parameter list
        int                 idxNext;        // 0x20
        int                 refCounters[8]; // 0x24
        unsigned int        age;            // 0x44
        unsigned int        threadMask;     // 0x48
    };

    // DB-verified (types_members dsEVENT_MGR::THREAD_ITEM_INFO): idxHandlerChain@0 — size 4.
    struct THREAD_ITEM_INFO {
        int idxHandlerChain; // 0x00
    };

    // DB-verified (types_members dsEVENT_MGR::PENDING_ITEM): idxEvent@0, idxNext@4, userId@8 — size 12.
    // While free, idxEvent overlays the pool's "next free index" link.
    struct PENDING_ITEM {
        int idxEvent; // 0x00
        int idxNext;  // 0x04
        int userId;   // 0x08
    };

    // DB-verified (types_members dsEVENT_MGR::ITEM_REC): name@0 (dsSTRID), threadInfo@4 — size 36.
    // While free, name.id overlays the pool's "next free index" link.
    struct ITEM_REC {
        dsSTRID          name;          // 0x00
        THREAD_ITEM_INFO threadInfo[8]; // 0x04
    };

    // Kind of subscriber stored in an ITEM_HANDLER (selects which handler-union arm is live).
    // ST_CALLBACK and ST_POLL are DB enumerator names; ST_HANDLER is the value-0 default arm (the
    // DB name for it was not surfaced). Values recovered from DispatchEvents: 0 = object handler
    // (pHandler->OnEvent), ST_CALLBACK = direct callback, ST_POLL = pooled/queued (idxSubscriber).
    enum SUBSCRIBER_TYPE {
        ST_HANDLER  = 0, // union arm: pHandler (dispatched via OnEvent)
        ST_CALLBACK = 1, // union arm: cbHandler (dispatched inline)
        ST_POLL     = 2, // union arm: idxSubscriber (queued onto the subscriber notify list)
    };

    // DB-verified (types_members dsEVENT_MGR::ITEM_HANDLER): type@0 (SUBSCRIBER_TYPE),
    // handler union@4 ($66FC5DE5..), userId@8, age@12, idxNext@16 — size 20.
    // While free, `type` overlays the pool's "next free index" link.
    struct ITEM_HANDLER {
        SUBSCRIBER_TYPE type;                       // 0x00
        union {                                     // 0x04 (DB anonymous union)
            dsEVENT_HANDLER *pHandler;              //   subscriber object
            void (*cbHandler)(const dsEVENT *);     //   subscriber callback
            int              idxSubscriber;         //   subscriber pool index
        };
        int          userId;  // 0x08
        unsigned int age;     // 0x0C
        int          idxNext; // 0x10
    };

    // DB-verified (types_members dsEVENT_MGR::SUBSCRIBER_REC): subscriber@0, refCnt@4,
    // idxNotifyHead@8, idxNotifyTail@12, threadId@16 — size 20.
    // While free, `subscriber` overlays the pool's "next free index" link.
    struct SUBSCRIBER_REC {
        const void *subscriber;    // 0x00
        int         refCnt;        // 0x04
        int         idxNotifyHead; // 0x08
        int         idxNotifyTail; // 0x0C
        int         threadId;      // 0x10
    };

    // Per-thread bookkeeping block (dsEVENT_MGR::THREAD_INFO, 332 bytes).
    // DB-verified (types_members dsEVENT_MGR::THREAD_INFO): isActive@0, age@4, isDispatching@8,
    // idxEventEmpty@12, numEmptyElements@16, idxEventHead@20, idxEventTail@24, idxThreadEvent[8]@28,
    // notifications@60 (dsPOOL<PENDING_ITEM,256,64>, 272).
    struct THREAD_INFO {
        int          isActive;          // 0x00 this thread participates in dispatch
        unsigned int age;               // 0x04 monotonically increasing signal age stamp
        int          isDispatching;     // 0x08 re-entrancy guard for DispatchEvents
        int          idxEventEmpty;     // 0x0C head of this thread's free EVENT list (-1 = none)
        int          numEmptyElements;  // 0x10 count of free EVENTs on idxEventEmpty
        int          idxEventHead;      // 0x14 head of this thread's pending EVENT chain
        int          idxEventTail;      // 0x18 tail cursor of this thread's pending EVENT chain
        int          idxThreadEvent[8]; // 0x1C per-target-thread event cursor
        dsPOOL<PENDING_ITEM, 256, 64> notifications; // 0x3C queued subscriber notifications
    };

    osLOCK lock;                                                          // 0x0000 (52)
    dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP> itemMap;    // 0x0034 (36) name -> handle
    dsPOOL<ITEM_REC, 256, 256>       items;                              // 0x0058 (1040)
    dsPOOL<ITEM_HANDLER, 512, 256>   handlers;                           // 0x0468 (1040)
    dsPOOL<EVENT, 1024, 512>         events;                             // 0x0878 (2064)
    ds::MAP<void const *, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> subscriberMap; // 0x1088 (84)
    dsPOOL<SUBSCRIBER_REC, 64, 64>   subscribers;                        // 0x10DC (272)
    THREAD_INFO                      threadInfo[8];                      // 0x11EC (2656)

    // 0x8261AA00 — register (or look up) the event named `name`: returns the existing handle when
    // already present, otherwise allocates an ITEM_REC, initialises its per-thread handler chains
    // to -1, and inserts it into `itemMap` under the lock (double-checked). Returns the handle (an
    // item-pool index).
    int RegisterEvent(dsSTRID name);

    // 0x82618E88 — raise the event with item handle `id` on the calling thread: grab a free EVENT
    // (refilling the per-thread free list of 32 under the lock when exhausted), stamp it with the
    // event id/age, compute the target-thread mask from the item's per-thread handler chains, copy
    // the parameter list into the EVENT (MT-safe deep-copy when cross-thread targets exist), link it
    // onto this thread's pending chain, and drive DispatchEvents unless already dispatching.
    void SignalEvent(int id, const dsPARAM_LIST *params);

    // 0x826159C8 — drain every thread's pending EVENT chains: for each active target thread, walk the
    // events queued for it, invoke each item's per-thread handler chain (object OnEvent / inline
    // callback / queued poll notification), record the notify count, and advance the per-(source,
    // target)-thread cursor. Runs under the calling thread's re-entrancy guard.
    void DispatchEvents();

    // (?SubscribeEvent@dsEVENT_MGR@@...) — add `handler` to event `id`'s per-thread (`userId`)
    // handler chain (ST_HANDLER arm). Body is a boundary (outside this re-source).
    void SubscribeEvent(int id, dsEVENT_HANDLER *handler, int userId);
    // (?UnsubscribeEvent@dsEVENT_MGR@@...) — remove `handler` from event `id`'s per-thread (`userId`)
    // handler chain. Body is a boundary (outside this re-source).
    void UnsubscribeEvent(int id, dsEVENT_HANDLER *handler, int userId);
} dsEVENT_MGR;
