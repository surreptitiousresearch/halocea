#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/PTR_LIST_NODE.h"
#include "../ds/dsTYPE_ID.h"
#include "msgRES.h"
// ws-engine msg: a message-system endpoint. Every actor that can send/receive engine messages
// is (or contains) an msgADDR: it carries an RTTI/dispatch vtable, an intrusive node in the
// active-address list, run/suspend state, a unique id, and a dispatch priority.
// DB-verified layout (types_members msgADDR):
//   __vftable@0, PTR_LIST_NODE@4, stateAddr@12, suspendState@16, uniqueID@20,
//   priority@24, priorityInt@28 — size 32.

struct msgADDR;
struct msgDATA;  // ../msg/msgDATA.h — init-message payload (pointer only, boundary)

// DB-verified dispatch vtable (types_members msgADDR_vtbl).
typedef struct msgADDR_vtbl {
    dsTYPE_ID   *(*TypeID)(msgADDR *self, dsTYPE_ID *result);              // 0x00
    void         (*dtr_msgADDR)(msgADDR *self);                           // 0x04
    msgRES       (*ProcessMsg)(msgADDR *self, int msg, void *pInfo, msgADDR *pSend);     // 0x08
    int          (*PreProcessMsg)(msgADDR *self, int msg, void *pInfo, msgADDR *pSend);  // 0x0C
    int          (*PostProcessMsg)(msgADDR *self, int msg, void *pInfo, msgADDR *pSend); // 0x10
    int          (*GetPriorityShift)(msgADDR *self);                      // 0x14
    unsigned int (*GetSuspendMask)(msgADDR *self);                        // 0x18
    void         (*OnChangeMode)(msgADDR *self, unsigned int, unsigned int);             // 0x1C
    void         (*OnSuspend)(msgADDR *self, int);                        // 0x20
    void         (*NotifyPostTermMsg)(msgADDR *self, msgADDR *);          // 0x24
} msgADDR_vtbl;

typedef struct msgADDR {
    msgADDR_vtbl            *__vftable;    // 0x00 dispatch/RTTI vtable
    ds::PTR_LIST_NODE       listNode;     // 0x04 hook in the message system's active-address list
    apSTATE_T<unsigned long> stateAddr;   // 0x0C run-state bitmask: bit 0x2 = not accepting messages,
                                          //      bit 0x8 = restrict to lifecycle events (msg 2/4),
                                          //      bit 0x20 = priority managed via change-priority list
    unsigned int            suspendState; // 0x10 per-address suspend mask
    int                     uniqueID;     // 0x14 stable id assigned by the message system
    unsigned int            priority;     // 0x18 caller-set dispatch priority
    unsigned int            priorityInt;  // 0x1C internal priority key (10 * priority when unpinned)

    // 0x8255D640 — set the unique id and (re)register it with the global message system.
    // Deviation: mangle is QAAXK_Z (`unsigned long` arg) — corrected from a prior `int` param.
    void SetUID(unsigned long uniqueID);
    // 0x8255BE78 — set the dispatch priority; recomputes priorityInt or re-sorts the change list.
    void SetPriority(unsigned int priority);
    // 0x8255B438 — enqueue this address for a priority re-sort on the message system.
    void ChangePriority();
    // 0x8255D808 — register this address with the global message system: deliver the init message
    // (1), and on success run the pending activation/render/spawn messages, link into the priority
    // buckets, notify subscribers, and sync the current mode. Returns true when registration took.
    bool Register(msgDATA *pInitData);

    // Runtime-type predicate: true when this address's dynamic type derives from T (its DESC RTTI
    // anchor). Member template, instantiated per queried type (e.g. msgADDR::IsA<entENTITY>
    // @0x83173DBC). boundary — body external.
    template<class T> bool IsA() const;
} msgADDR;
