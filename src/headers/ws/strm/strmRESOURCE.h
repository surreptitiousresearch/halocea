#pragma once
#include "STRM_QUEUE_ID.h"
// ws-engine strm (streaming): abstract base for a streamed/queryable asset (a resource that can
// be enqueued into a strmSTREAMING_QUEUE and progresses through the STATE state machine below).
// DB-verified layout (types_members strmRESOURCE): __vftable@0, state@4 — size 8.
// DB-verified vtable (types_members strmRESOURCE_vtbl).

// strmPRIORITY — union, size 8 (DB types_members strmPRIORITY): an anonymous
// struct { int global; int local; } aliased with an __int64 `value`. Only used
// as a by-value vtable return type here (GetPriority), not part of this struct's
// layout, so it is left as an opaque forward declaration; full layout is DB-known.
union strmPRIORITY;

struct strmRESOURCE_vtbl;

typedef struct strmRESOURCE {
    // strmRESOURCE::STATE — DB-verified (types_enum_values strmRESOURCE::STATE).
    enum STATE {
        RES_STATE_IDLE     = 0,
        RES_STATE_IN_QUERY = 1,
        RES_STATE_LOADING  = 2,
        RES_STATE_READY    = 3,
        RES_STATE_CANCEL   = 4,
    };

    strmRESOURCE_vtbl *__vftable; // 0x00
    int                 state;     // 0x04 (a strmRESOURCE::STATE value)

    ~strmRESOURCE();                                    // vtbl+0x00
    strmPRIORITY GetPriority();                         // vtbl+0x04
    STRM_QUEUE_ID GetQueueId();                         // vtbl+0x08
    bool OnAddQuery();                                  // vtbl+0x0C
    bool OnBeforeReadData();                             // vtbl+0x10
    bool OnReadData();                                  // vtbl+0x14
    void SetState(int newState);                        // vtbl+0x18
} strmRESOURCE;
