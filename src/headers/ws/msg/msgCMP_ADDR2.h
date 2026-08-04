#pragma once
#include "../ds/PTR_LIST.h"
// ws-engine msg: stateless comparator functor that orders the message system's priority buckets
// (each a ds::PTR_LIST<msgADDR>) against one another by their head address's internal priority
// key. Empty struct (DB: msgCMP_ADDR2 has no data members).

struct msgADDR;

typedef struct msgCMP_ADDR2 {
    // 0x825587F8 — three-way compare of two priority buckets by their head element's priority key:
    // an empty list sorts before a non-empty one (-1 / +1); two non-empty lists compare their head
    // priorityInt; two empty lists are equal (0).
    unsigned int cmp(const ds::PTR_LIST<msgADDR> *list1, const ds::PTR_LIST<msgADDR> *list2) const;
} msgCMP_ADDR2;
