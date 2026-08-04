#pragma once
// ws-engine ds: ref-counted weak reference. The shared handle's pPtr is nulled when the
// referent dies. DB-verified layout (types_members ds::WEAK_PTR<entENTITY>): pHandle@0.

namespace ds {

template<class T>
struct WEAK_PTR_HANDLE {
    T           *pPtr;     // 0x00 — DB types_members ds::WEAK_PTR_HANDLE<T>: pPtr@0, refCount@4
    volatile int refCount; // 0x04
};

template<class T>
struct WEAK_PTR {
    WEAK_PTR_HANDLE<T> *pHandle; // 0x00

    // 0x823CF3CC (WEAK_PTR<entENTITY>) — null pHandle (adopts no referent). Reversed
    // per-instantiation.
    WEAK_PTR();
    WEAK_PTR(const WEAK_PTR &other); // boundary — external to this batch
    // 0x823D575C (calls SetPtr(pPtr) — reversed here). Body: SetPtr(pPtr).
    explicit WEAK_PTR(T *pPtr);
    // 0x823D8024-region dtor — calls Release(). boundary — external to this batch as a
    // separate body (mirrors Release's).
    ~WEAK_PTR();

    // 0x823D39CC — drop this handle's reference (osLockedDecrement; free the handle once its
    // refcount hits 0), then null pHandle. Reversed per-instantiation.
    void Release();
    // 0x823D575C — Release() the current handle, then, if `pPtr` is non-null, adopt
    // `pPtr->GetHandle()` (bumping its refcount). Reversed per-instantiation.
    void SetPtr(T *pPtr);

    T *GetPtr() const;                 // boundary — external to this batch
    T *operator->() const;             // boundary
    bool operator==(const WEAK_PTR &o) const; // boundary
    WEAK_PTR &operator=(const WEAK_PTR &o);   // boundary
};

} // namespace ds
