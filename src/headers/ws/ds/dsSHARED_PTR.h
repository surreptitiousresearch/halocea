#pragma once
#include "Deleter.h"   // DEL template param is a Deleter<T> policy; couple it so consumers resolve it
// ws-engine ds: intrusive shared (ref-counted, non-owning-of-policy) pointer. Unlike dsSMART_PTR
// the referent count lives beside the pointer in the handle itself.
// DB-verified layout (types_members dsSHARED_PTR<animLAYER_BASE,0,Deleter<animLAYER_BASE> >):
//   pointee@0 (T*), counter@4 (int) — size 8.
// Template params: T = referent type; N = policy discriminator; DEL = deleter policy.

template<class T, int N, class DEL>
struct dsSHARED_PTR {
    T   *pointee; // 0x00 referent
    int  counter; // 0x04 shared reference count

    // Release the current referent (decrement the shared counter and invoke the deleter at zero),
    // then null out. Body external to this batch. // boundary
    void Reset();

    // Release the current referent, then adopt `p` (of a derived type U) with a fresh shared count.
    // DB mangle: ?Reset@?$dsSHARED_PTR@...@@QAAXPAV<U>@@@Z. Body external to this batch. // boundary
    template<class U> void Reset(U *p);

    // Assign from another shared pointer: release the current referent, adopt `other`'s and bump
    // the shared count. DB mangle ?4?$dsSHARED_PTR@...@@QAAAAV0@ABV0@@Z. Body external. // boundary
    dsSHARED_PTR<T, N, DEL> &operator=(const dsSHARED_PTR<T, N, DEL> &other);

    // Copy-construct from another shared pointer: adopt its pointee/counter pointer and bump the
    // shared count. Body external to this batch. // boundary
    dsSHARED_PTR(const dsSHARED_PTR<T, N, DEL> &other);
    dsSHARED_PTR(); // boundary (empty)
    // Adopt `p` with a fresh shared count. DB mangle: ?$dsSHARED_PTR@...::dsSHARED_PTR(this, T*).
    // Body external to this batch. // boundary
    explicit dsSHARED_PTR(T *p);
};
