#pragma once
#include "dsVECTOR.h"
#include "../../apCL.h"
// ws-engine ds: intrusive doubly-linked list of VALUES (not pointers) backed by a single growable
// pool vector — nodes are never individually heap-allocated; freed slots are recycled via an
// intrusive free-list threaded through the same RAW_NODE storage. DB-verified layout
// (types_members ds::VAL_LIST<dsSHARED_COUNTER>): usedHead@0, usedTail@4, freeHead@8, pool@12
// (dsVECTOR<RAW_NODE,8>), cl@32 (apCL) — size 40.
//
// RAW_NODE overlays two shapes depending on whether the slot is live or on the free list: live
// slots store {prevIdx, nextIdx, T val}; free slots only use the first int as the singly-linked
// free-list "next" pointer. This dual shape is exactly what ds_list.cpp's raw byte-array (`t[]`)
// punning in the decompiled AllocNode/Erase/InsertAfter/Clear reflects. Modeled as a NODE view
// (the live shape) since every reversed caller in this batch only ever touches nextIdx/prevIdx/val
// through it.

namespace ds {

template<class T>
struct VAL_LIST {
    struct NODE {
        int prevIdx; // 0x00 (or: singly-linked free-list "next" index, when this slot is free)
        int nextIdx; // 0x04
        T   val;     // 0x08
    };
    typedef NODE RAW_NODE; // pool storage element (same layout; distinguished only by use)

    struct ITERATOR {
        int                          elemIdx; // 0x00 index into pool, or -1 (end)
        dsVECTOR<RAW_NODE, 8>       *pool;    // 0x04 owning pool (for Get()/dereference)
    };
    struct C_ITERATOR {
        int                          elemIdx; // 0x00
        const dsVECTOR<RAW_NODE, 8> *pool;    // 0x04
    };

    int                    usedHead; // 0x00 index of the first live node, or -1
    int                    usedTail; // 0x04 index of the last live node, or -1
    int                    freeHead; // 0x08 index of the first free (recycled) slot, or -1
    dsVECTOR<RAW_NODE, 8>  pool;     // 0x0C backing storage for every live/free node
    apCL                   cl;       // 0x20 allocation call-site cookie

    VAL_LIST(const apCL &cl); // boundary — external to this batch
    ~VAL_LIST();              // boundary — external to this batch

    // 0x8267DFE8 (VAL_LIST<dsSHARED_COUNTER>) — recycle a slot from `freeHead` if one is free,
    // else grow `pool` by one and use the new slot; stores `*val` into the slot's `val` member
    // (freshly-grown-slot path constructs it via PushBack; recycled-slot path assigns it
    // directly). Returns a pointer to the slot's NODE view. boundary — the raw decompile's
    // NODE/RAW_NODE byte-array punning (`v9.t[8]`, `result->val = (dsSHARED_COUNTER)val->used`)
    // is too lossy to safely re-derive the exact per-type assignment semantics; declared here
    // with its accurate control-flow summary rather than forced into unreliable C++.
    NODE *AllocNode(const T &val); // boundary

    // 0x82601... (VAL_LIST<dsSHARED_COUNTER>) — unlink the node at `where` from the used
    // doubly-linked list (patching its neighbours' prev/next, and usedHead/usedTail if it was an
    // endpoint), then push its slot onto the free list (`freeHead`). Two overloads: by iterator-
    // range (`Erase(ITERATOR first, ITERATOR last)`, only `first` observed as used with a
    // zero-length virtual "last") and by (iterator, count). boundary — see AllocNode's note.
    ITERATOR Erase(ITERATOR first, ITERATOR last); // boundary
    ITERATOR Erase(ITERATOR where, int count);     // boundary (delegates to the range overload)

    // 0x8267E330 (VAL_LIST<dsSHARED_COUNTER>) — AllocNode(val), then splice the new node into the
    // used list immediately after `where` (or at the tail, when `where` is the end iterator);
    // patches usedHead/usedTail/prevIdx/nextIdx accordingly. boundary — see AllocNode's note.
    ITERATOR InsertAfter(const T &val, ITERATOR where); // boundary

    ITERATOR PushBack(const T &val); // boundary — external to this batch
    ITERATOR Begin();                // boundary
    ITERATOR Head();                 // boundary
    ITERATOR Tail();                 // boundary
    int      Capacity() const;       // boundary

    // 0x82730F18 (VAL_LIST<scrSCORCH_MARK_DYN>) — walk the used list from `usedHead`, unlinking
    // every node into the free list; for each unlinked node, if its `val` carries a non-null
    // refcounted-object pointer (observed as an extra field beyond `prevIdx`/`nextIdx` — the
    // scrSCORCH_MARK_DYN instantiation's `val` embeds a ds::dsSHARED_PTR-style owned pointer),
    // osLockedDecrement its refcount and destroy it once the count reaches 0, then null the
    // slot's owned-pointer field. boundary — the exact byte-offset semantics of that extra field
    // (t+8 in the raw decompile) are specific to scrSCORCH_MARK_DYN's own (not-yet-reversed)
    // layout, so re-deriving a generic-template C++ body here would risk silently wrong ownership
    // transfer; the control-flow summary above is DB-verified, the byte-level body is not.
    void Clear(); // boundary
};

} // namespace ds
