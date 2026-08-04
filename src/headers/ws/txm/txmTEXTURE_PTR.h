#pragma once
#include "txmTEXTURE.h"
// ws-engine txm: a ref-counting smart pointer wrapping a txmTEXTURE*.
// DB-verified layout (types_members txmTEXTURE_PTR): ptr@0 — size 4.

typedef struct txmTEXTURE_PTR {
    txmTEXTURE *ptr; // 0x00

    // boundary — copy-assign; external to this batch (rebinds ptr, adjusting refcounts).
    txmTEXTURE_PTR &operator=(const txmTEXTURE_PTR &other);

    // 0x823EF49C -- reversed in txmTEXTURE_PTR__GetPtr.c. Plain accessor for `ptr`.
    // NOTE: no distinct `operator*` (dereference) was found in the database for this type --
    // only GetPtr (above), the implicit `operator txmTEXTURE*` conversion (??B), and
    // `operator->` (??C) exist as compiled symbols. GetPtr is the closest equivalent.
    txmTEXTURE *GetPtr() const;
} txmTEXTURE_PTR;
