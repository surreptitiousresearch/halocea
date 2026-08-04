#pragma once
// ws-engine ds: intrusive ref-counting owning smart pointer. The ownership/lifetime policy
// is supplied as an empty base class (POLICY) at offset 0, so the object is pointer-sized.
// DB-verified layout (types_members dsSMART_PTR<dscDESC,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>>):
//   _dsSP_OWNER_PLC_REFCOUNT<...>@0 (empty base), pointee@0 (T*) — size 4.

// Empty ownership policy: releases a reference on the pointee and, on last release, invokes
// the pointee's virtual deleting destructor. Carries no state (empty base optimization).
template<class PTR>
struct _dsSP_OWNER_PLC_REFCOUNT {};

template<class T, class POLICY>
struct dsSMART_PTR : POLICY {
    T *pointee; // 0x00 owned referent (nullptr, or the -4 sentinel for a not-yet-resolved slot)

    // 0x823D1A10 / 0x8253D3C0 — release the owned reference: decrement the referent's intrusive
    // count and, if this was the last owner, call its virtual deleting destructor; then null out.
    void DeletePointee();

    // 0x825E2D90 (dsSMART_PTR<rendLIGHT_SET,...>) — copy-construct from another smart pointer:
    // adopt its pointee and bump the intrusive count (unless null or the -4 unresolved sentinel).
    dsSMART_PTR(const dsSMART_PTR<T, POLICY> &other);
    dsSMART_PTR();                                     // boundary (empty)

    // 0x825E3828 (dsSMART_PTR<rendLIGHT_SET,...>) / 0x825D6480 (dsSMART_PTR<dscDESC,...>) — release
    // the current referent, then adopt `other`'s pointee (bumping its count unless null/sentinel).
    dsSMART_PTR<T, POLICY> &operator=(const dsSMART_PTR<T, POLICY> &other);
};
