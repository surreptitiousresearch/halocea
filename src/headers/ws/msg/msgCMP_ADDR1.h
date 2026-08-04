#pragma once
#include "../ds/PTR_LIST.h"
// ws-engine msg: stateless comparator functor that orders a priority-sorted address list by
// each address's internal priority key. Empty struct (DB: msgCMP_ADDR1 has no data members).

struct msgADDR;

typedef struct msgCMP_ADDR1 {
    // 0x82558790 — three-way compare of a priority-sorted list's head priority against pAddr's.
    unsigned int cmp(const ds::PTR_LIST<msgADDR> *list, msgADDR *pAddr) const;
} msgCMP_ADDR1;
