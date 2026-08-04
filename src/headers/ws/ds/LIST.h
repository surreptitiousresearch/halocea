#pragma once
#include "CMP.h"
#include "../../apCL.h"
// ws-engine ds: value-owning doubly-linked list (each DATA node stores a copy of the element),
// distinct from ds::PTR_LIST (intrusive). DB-verified layout (types_members ds::LIST<msgADDR *>):
//   length@0, head@4, tail@8, __cl@12 (apCL) — size 20; DATA = {val@0, next@4, prev@8}.

namespace ds {


template<class T>
struct LIST {
    // A heap-allocated list cell holding one element value.
    struct DATA {
        T     val;  // 0x00 stored element
        DATA *next; // 0x04 successor cell
        DATA *prev; // 0x08 predecessor cell

        // Constructs a detached cell: val = value, next = prev = nullptr.
        DATA(T const &value);
    };

    // A position handle over the list; wraps a single DATA pointer.
    struct ITERATOR {
        DATA *data; // 0x00

        ITERATOR();                      // data = nullptr (end/default iterator)
        ITERATOR(const ITERATOR &other); // data = other.data
        bool  IsDone() const;            // data == nullptr
        DATA *operator*() const;         // returns data
        void  operator++();              // data = data ? data->next : data

    private:
        friend struct LIST<T>;
        // Private: only LIST<T> may hand out an iterator wrapping a specific cell.
        ITERATOR(DATA *fromData);
    };

    int   length; // 0x00 live element count
    DATA *head;   // 0x04 first cell
    DATA *tail;   // 0x08 last cell
    apCL  __cl;   // 0x0C allocation call-site cookie {file, line}

    // Construct empty; records the allocation call-site cookie for later PushBack allocations.
    LIST(apCL cl);
    // Copy-construct: deep-copy every cell of `other`, in list order, via PushBack.
    LIST(const LIST<T> &other);
    // Destroy every cell.
    ~LIST();

    // Self-assignment safe: Clear() then deep-copy-construct from `other` in place.
    // DB mangle (QAAXABV01@) marks the return type void, not LIST& — match it.
    void operator=(const LIST<T> &other);
    // Append `val` to the tail; equivalent to PushBack(val).
    LIST<T> &operator+=(T const &val);

    bool IsEmpty() const; // head == nullptr

    // Unlink the cell referred to by `where` and free it; relinks neighbors (or head/tail when
    // `where` was an end cell); decrements length; returns an iterator to the following cell.
    ITERATOR Erase(ITERATOR where);

    // Overload (DB: ?Erase@?$LIST@...@ds@@QAAHABQAV...@Z) — find the first cell whose value equals
    // `val`, unlink and free it; returns non-zero when a cell was removed. // boundary
    int Erase(const T &val);

    // Append a new heap cell holding a copy of `val` to the tail (or as the sole cell when empty).
    void PushBack(T const &val);

    // Destroy every cell and reset to empty (length = 0, head = tail = nullptr).
    void Clear();

    // Iterator to the first cell (or an end iterator when empty). Two overloads (const/non-const)
    // share identical logic: result.data = head.
    ITERATOR Begin() const;
    ITERATOR Begin();
};

// ds::FindNonSorted — linear scan from `start` for the first cell whose value equals `key`,
// using an explicit comparator type CMP; returns an iterator to it (or an end iterator on miss).
template<class CMP, class IT, class VAL>
IT FindNonSorted(const IT &start, const VAL &key);

// ds::FindNonSorted — overload using the default comparator (ds::CMP); forwards to the
// 3-template-arg form above.
template<class IT, class VAL>
IT FindNonSorted(const IT &start, const VAL &key);

} // namespace ds
