#pragma once
// ws-engine ds: intrusive doubly-linked list of pointers, parameterised by a node-access
// policy. DB-verified layout (types_members ds::PTR_LIST<...>): pHead@0, pTail@4, length@8.

namespace ds {

// Default node-access policy: assumes T embeds a `ds::PTR_LIST_NODE`-shaped {pPrev,pNext} pair (or,
// for allocator free-list nodes, a compatible {pNext@0,pPrev@4} pair) directly at offset 0, and
// simply reads/writes those fields. DB-verified per-instantiation (e.g.
// LIST_NODE_ACCESS_POLICY<ds::MAP<int,msgADDR*,...>::DUMMY>::Next/Prev/SetNext/SetPrev, all of which
// just forward to `pNode->pNext`/`pNode->pPrev`).
template<class T>
struct LIST_NODE_ACCESS_POLICY {
    static T *Next(T *pNode);             // pNode->pNext
    static T *Prev(T *pNode);             // pNode->pPrev
    static void SetNext(T *pNode, T *pNext); // pNode->pNext = pNext
    static void SetPrev(T *pNode, T *pPrev); // pNode->pPrev = pPrev
};

template<class T, class POLICY = LIST_NODE_ACCESS_POLICY<T> >
struct PTR_LIST {
    // A position handle over the list; wraps a single node pointer at offset 0.
    struct ITERATOR {
        T *pNode; // 0x00

        ITERATOR() : pNode(0) {}
        ITERATOR(T *node) : pNode(node) {}

        // Reversed per-instantiation (dereference/advance the handle).
        T &operator*() const;
        T *operator->() const; // msgADDR instantiation only (DB-verified real compiled body)
        T *Get() const;
        ITERATOR &operator++();
    };
    // Const-position handle; same single-pointer layout as ITERATOR.
    struct C_ITERATOR {
        T *pNode; // 0x00

        C_ITERATOR() : pNode(0) {}
        C_ITERATOR(T *node) : pNode(node) {}

        bool IsDone() const { return pNode == 0; }
        const T &operator*() const;
        const T *operator->() const;
        C_ITERATOR &operator++();
    };

    T  *pHead;  // 0x00
    T  *pTail;  // 0x04
    int length; // 0x08

    // Default-construct an empty list. Reversed per-instantiation.
    PTR_LIST();

    // true when the list has no nodes. Reversed per-instantiation.
    bool IsEmpty() const;
    // Iterator to the first / last node. Reversed per-instantiation.
    ITERATOR Front();
    ITERATOR Back();
    // Const-iterator to the first node (msgADDR instantiation only). Reversed per-instantiation.
    C_ITERATOR FrontConst();

    // Append `val` at the tail; returns an iterator to it. Reversed per-instantiation.
    ITERATOR PushBack(T *val);
    // Prepend `val` at the head; returns an iterator to it (msgADDR instantiation only). Reversed
    // per-instantiation.
    ITERATOR PushFront(T *val);
    // Return every node's ownership to the caller without freeing them (unlinks all nodes and
    // resets pHead/pTail/length). Reversed per-instantiation.
    void Clear();

    // Unlink the node the iterator refers to. Reversed per-instantiation (per corpus convention,
    // return type/exact signature verified against each instantiation's mangle: msgADDR's Erase
    // returns the unlinked T*; the allocator-DUMMY's Erase also returns the unlinked T*).
    T *Erase(ITERATOR it);

    // Splice `val` into the list at `where` (the position to insert before / the tail hook when
    // `where` wraps the current tail); returns an iterator to the inserted node. Reversed
    // per-instantiation.
    ITERATOR Insert(T *val, ITERATOR where);
};

} // namespace ds
