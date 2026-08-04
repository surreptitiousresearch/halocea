#pragma once
#include "WEAK_PTR.h"
// ws-engine ds: base-class mixin that gives a type a weak-referenceable identity — an owned
// handle (refcount + back-pointer) that observers can hold via ds::WEAK_PTR<T> without keeping
// the referent alive; the handle's pPtr is nulled when the referent is destroyed.
// DB-verified layout (types_members ds::WEAK_PTR_BASE<iaIACTOR>, also propBASE):
//   pHandle@0 (ds::WEAK_PTR_HANDLE<T> *) — size 4.

namespace ds {

template<class T>
struct WEAK_PTR_BASE {
    WEAK_PTR_HANDLE<T> *pHandle; // 0x00 owned weak-reference handle (lazily allocated)

    // 0x823D1BE0 (WEAK_PTR_BASE<iaIACTOR>) — lazily allocate pHandle the first time it's needed:
    // {refCount=1, pPtr=this's owning T*}. Returns the (existing or freshly-allocated) handle.
    // Reversed per-instantiation.
    WEAK_PTR_HANDLE<T> *GetHandle();
};

} // namespace ds
