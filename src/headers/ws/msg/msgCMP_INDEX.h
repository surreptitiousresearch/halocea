#pragma once
// ws-engine msg: stateless comparator functor that orders/searches a msgSUBSCRIBER's
// saLIST table by message id. Empty struct (DB: msgCMP_INDEX has no data members).

struct saLIST;

typedef struct msgCMP_INDEX {
    // 0x82555F20 — three-way compare of a pair-list entry's key against a message id.
    int cmp(const saLIST *list, int msg) const;
} msgCMP_INDEX;
