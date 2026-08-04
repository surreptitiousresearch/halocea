#pragma once
// ws-engine ds: intrusive doubly-linked-list hook embedded in objects tracked by ds::PTR_LIST.
// DB-verified layout (types_members ds::PTR_LIST_NODE): pPrev@0, pNext@4 — size 8.

namespace ds {

typedef struct PTR_LIST_NODE {
    PTR_LIST_NODE *pPrev; // 0x00 previous node in the list
    PTR_LIST_NODE *pNext; // 0x04 next node in the list
} PTR_LIST_NODE;

} // namespace ds
