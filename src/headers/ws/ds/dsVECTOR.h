#pragma once
#include "../../apCL.h"
// ws-engine ds: growable array — the workhorse container (~285 instantiations in this binary).
// DB-verified layout (types_members dsVECTOR<inpDEVICE *,8>):
//   pData@0, nElem@4, allocated@8, __cl@12 (apCL) — size 20.
// Template params: T = element type; N = integer growth/alignment granularity.

template<class T, int N>
struct dsVECTOR {
    // Duplicate-key policy for InsertSorted. DB-verified values (types_enum_values
    // dsVECTOR<...>::INS_SORT): REPLACE=0, IGNORE=1, ERROR=2, RIGHT=3.
    enum INS_SORT {
        INS_DUP_REPLACE = 0, // overwrite the matching element's value; return its index
        INS_DUP_IGNORE  = 1, // keep the existing element unchanged; return its index
        INS_DUP_ERROR   = 2, // a duplicate is a contract violation (asserts / returns -1)
        INS_DUP_RIGHT   = 3, // insert the new element after the match; return the match index
    };

    T    *pData;     // 0x00 heap element array
    int   nElem;     // 0x04 number of live elements
    int   allocated; // 0x08 capacity
    apCL  __cl;      // 0x0C allocation call-site cookie {file, line}

    // Default-construct an empty vector (null storage, zero count). Body not decompiled here. boundary.
    dsVECTOR(); // boundary

    // 0x82516598 (dsVECTOR<psSECTION_RECORD,8>) / 0x82680E50 (dsVECTOR<mtlMTL,8>) / 0x82516520
    // (dsVECTOR<psSECTION,8>) — construct an empty vector, adopting the allocation call-site
    // cookie `cl` (copied by value into __cl).
    dsVECTOR(const apCL &cl);

    // 0x828623D0 (dsVECTOR<m3dMATR,8>) — construct with an initial live size: adopts `cl`, then
    // delegates to Resize(size) (default-filling/growing to `size` elements).
    dsVECTOR(int size, const apCL &cl);

    // 0x8255BB90 (dsVECTOR<ds::MAP<int,msgADDR*,...>::ITEM*,8>) — construct with an initial live
    // size, filling every slot from `fill`: adopts `cl`, then delegates to Resize(size, fill).
    // Distinct from the 2-arg (size, cl) ctor above (mangle carries an extra `const T&` param).
    // Reversed per-instantiation.
    dsVECTOR(int size, const T &fill, const apCL &cl);

    // 0x823EEB28 (dsVECTOR<m3dMATR,8>) / 0x827EE458 (dsVECTOR<dsPARAM,8>) — copy-construct: adopt
    // src's call-site cookie, then Insert the full source range. Reversed per-instantiation.
    dsVECTOR(const dsVECTOR<T, N> &src);

    // 0x823C18F8 (dsVECTOR<inpDEVICE*,8>) / 0x82680A88 (dsVECTOR<mtlMTL,8>) / 0x82516030
    // (dsVECTOR<psSECTION,8>) — number of live elements.
    int Length() const;

    // 0x823CF3B0 (dsVECTOR<propBASE*,8>) — true when nElem == 0.
    bool IsEmpty() const;

    // 0x823F1F60 (dsVECTOR<apDEFRAG_CHUNK_DESC*,8>) / 0x82688470 (dsVECTOR<mtlMTL,8>) / 0x8251BC20
    // (dsVECTOR<psSECTION,8>) etc. — grow the backing store to at least `size` elements if it
    // isn't already that large (delegates to Realloc); a no-op otherwise.
    void Reserve(int size);

    // 0x82519230 (dsVECTOR<psSECTION_RECORD,8>) / 0x82688298 (dsVECTOR<mtlMTL,8>) / 0x8251BC38
    // (dsVECTOR<psSECTION,8>) — shrink the backing store to exactly fit the live elements
    // (delegates to Realloc(nElem)).
    void Compact();

    // 0x82684458 (dsVECTOR<mtlMTL,8>) — destroy every live element and reset the count.
    void Clear();

    // 0x82517690 (CallNew<psSECTION_RECORD>) / 0x82719320 (CallNew<apCOUNTER_UNIT>) /
    // 0x828600E0 (CallNew<m3dMATR>) — placement-"construct" helper: default-constructs the
    // element fields at raw slot `t` in place and returns it (null-t is a no-op passthrough).
    // Reversed per-instantiation.
    template<class U>
    U *CallNew(U *t);

    // Element access / append.
    // 0x82C2B0E8 (dsVECTOR<mtlMTL,8>).
    const T &operator[](int index) const; // boundary (reversed per-instantiation)
    // 0x82685E20 (dsVECTOR<mtlMTL,8>).
    T &operator[](int index);             // boundary (reversed per-instantiation)
    // 0x82C2CEF8 (dsVECTOR<mtlMTL,8>) — returns the newly appended slot (via Back()).
    T &PushBack(const T &value);           // boundary (reversed per-instantiation)

    // 0x826889D0 (dsVECTOR<dsTSTRING<char>,8>) — thunk straight to PushBack(value); append and
    // return the newly appended slot.
    T &operator+=(const T &value);

    // 0x825CD4D0 / 0x82685950 — shrink the live count to `size`, destroying the elements above
    // it (no reallocation). Asserts size >= 0.
    void ShrinkResize(int size);

    // Grow/shrink the backing storage to hold `capacity` elements; body not decompiled here.
    void Realloc(int capacity);            // boundary
    // Grow the storage to `capacity` elements (used by dsDEQUE::ResizeBack); body not decompiled here.
    void Resize(int capacity);             // boundary

    // 0x82522700 (dsVECTOR<bool,8>) etc. — resize the live count to `size`, copy-constructing any
    // newly exposed elements from `fill`. Reversed per-instantiation.
    void Resize(int size, const T &fill);

    // Swap all three fields (pData/nElem/allocated) with another vector.
    void Swap(dsVECTOR<T, N> &other);
    // 0x82AC8A18 (dsVECTOR<unsigned char,8>) etc. — splice `count` elements from `begin` in at
    // index `pos`: grow to nElem+count (doubling policy), shift the tail up by `count`, then copy
    // the range in (adjusting `begin` if it aliased storage that was reallocated). Reversed
    // per-instantiation.
    void Insert(int pos, const T *begin, int count);

    // 0x823B7460 (dsVECTOR<int,8>) / 0x8269A608 (dsVECTOR<strmVID_BUFFER*,8>) /
    // 0x82C2CD38 (dsVECTOR<mtlMTL,8>) — fill overload:
    // splice `count` copies of the single element `val` in at index `pos` (grow-and-shift, then
    // assign each new slot). Reversed per-instantiation for trivially-copyable element types.
    void Insert(int pos, const T &val, int count);

    // 0x823DACD8 (dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>) / 0x82C2AFE8 (dsVECTOR<mtlMTL,8>) —
    // remove `n` elements starting at index
    // `pos` by shifting the tail down over them (memmove) and dropping the live count. Assumes
    // trivially-relocatable elements (no per-element destructor is run).
    void Erase(int pos, int n);

    // ?EraseNonSorted@?$dsVECTOR@UHALO_CHANNEL@@$07@@QAAXH@Z /
    // ?EraseNonSorted@?$dsVECTOR@UHALO_CUSTOM_SOUND@@$07@@QAAXH@Z — unordered-remove: overwrite
    // the element at `pos` with the current last element (Back()), then drop the live count by
    // one. O(1) but does not preserve element order — used by callers compacting a vector while
    // iterating it front-to-back (they step their loop index back by one after calling this).
    // Body external to this batch (added by the HALO_SOUND_SYSTEM drain). boundary.
    void EraseNonSorted(int pos);

    // 0x8269BD10 / 0x823B7B40 / 0x829115D0 (dsVECTOR<psSECTION,8>, PRED=TEMPLATE_CMP) —
    // binary-search insert into an ascending-sorted vector using PRED for ordering. On a miss,
    // splices `val` at the lower-bound position and returns it. On a match, `dup` selects
    // overwrite / ignore / duplicate-right (see INS_SORT); an unrecognized `dup` returns -1.
    template<class PRED>
    int InsertSorted(const T &val, const PRED &cmp, INS_SORT dup);

    // 0x823E2010 (dsVECTOR<dsTSTRING<char>,8>) — convenience overload: default-constructs a
    // stateless dsCMP and delegates to InsertSorted(val, cmp, dup).
    int InsertSorted(const T &val, INS_SORT dup);

    // Destroy every live element and release the backing storage (destructor path).
    // 0x82684A70 (dsVECTOR<mtlMTL,8>).
    ~dsVECTOR();                           // boundary (reversed per-instantiation)

    // 0x826244D0 (dsVECTOR<dsTSTRING<char>,8>) — compiler-generated scalar deleting destructor
    // (`??_G` mangle): runs ~dsVECTOR(), then conditionally operator-deletes `this` when bit 0 of
    // `deleteFlags` is set.
    dsVECTOR<T, N> *scalarDeletingDtor(unsigned char deleteFlags);

    // 0x8252A2A8 (dsVECTOR<...ERASED,8>) — reference to the first live element (asserts nElem>0).
    T &Front();

    // 0x823B56F8 etc. (many instantiations) — reference to the last live element
    // (the slot at index nElem-1); asserts nElem>0 first.
    T &Back();
    // 0x82AF12C0 (dsVECTOR<sslSTACK_DATA,8>) — const overload of Back().
    const T &Back() const;

    // 0x82AC3AC8 (dsVECTOR<msgMSG,8>) / 0x825233B0 (dsVECTOR<dsDATA,8>) — raw pointer to the first
    // element slot (pData passthrough).
    T *Begin();
    // 0x827AE708 (dsVECTOR<dsDATA,8>) — const overload of Begin().
    const T *Begin() const;

    // 0x82632E78 (dsVECTOR<dsTSTRING<char>,8>) — raw pointer one-past the last live element
    // (&pData[nElem]).
    const T *End() const;
    // 0x82BDCD00 (dsVECTOR<dsTSTRING<char>,8>) — non-const overload of End().
    T *End();

    // 0x832E19F8 (dsVECTOR<dsTSTRING<char>,8>) — fully release the vector: destroy every live
    // element and free the backing storage (delegates to Realloc(0)). Distinct from the destructor
    // only in that the vector remains valid/reusable afterward (pData/nElem/allocated all zeroed).
    void Free();

    // 0x82AF1340 (dsVECTOR<sslSTACK_DATA,8>) — drop the last live element via ShrinkResize(nElem-1);
    // a no-op on an empty vector.
    void PopBack();

    // Copy-assign every element from another vector.
    // 0x82689E40 (dsVECTOR<mtlMTL,8>).
    dsVECTOR<T, N> &operator=(const dsVECTOR<T, N> &other); // boundary (reversed per-instantiation)

    // 0x823D4140 (dsVECTOR<dsPAIR<unsigned long,HCEX_MODEL>,8>) — linear scan from startPos
    // for the first element whose CMP key equals `key`; returns its index or -1.
    template<class CMP, class KEY>
    int Find(const KEY &key, const CMP &cmp, int startPos) const;

    // 0x82703D10 (dsVECTOR<camCAMERA*,8>) — convenience overload: default-constructs the
    // stateless CMP and delegates to the (cmp, startPos) form starting from index 0.
    template<class KEY>
    int Find(const KEY &key) const;

    // 0x823D4090 etc. / 0x82906AF8 (dsVECTOR<psSECTION,8>, CMP=TEMPLATE_CMP, KEY=dsSTRID) —
    // binary search of an ascending-sorted vector for the element whose CMP key equals `key`;
    // returns its index or -1.
    template<class CMP, class KEY>
    int FindSorted(const KEY &key, const CMP &cmp) const;

    // 0x82602C58 (dsVECTOR<dsTSTRING<char>,8>) — convenience overload: default-constructs a
    // stateless dsCMP and delegates to FindSorted(key, cmp).
    template<class KEY>
    int FindSorted(const KEY &key) const;

    // 0x8251A0A0 (dsVECTOR<psSECTION_RECORD,8>) — in-place sort ordered by `cmp` and exchanging
    // elements via `swap`. Uses an O(n^2) selection sort for up to 100 elements; delegates to
    // HeapSort for larger vectors. Reversed per-instantiation.
    template<class Cmp, class Swap>
    void Sort(const Cmp &cmp, const Swap &swap);

    // 0x8261C078 (dsVECTOR<dsDATA,8>::Sort<dsREF_VECTOR::SortCmp>) — convenience overload:
    // default-constructs a stateless dsSWAP<T> and delegates to Sort(cmp, swap).
    template<class Cmp>
    void Sort(const Cmp &cmp);

    // 0x82664B20 (dsVECTOR<dsTSTRING<char>,8>) — convenience overload: default-constructs a
    // stateless dsCMP and dsSWAP<T>, then delegates to Sort(cmp, swap).
    void Sort();

    // 0x8263AD48 (dsVECTOR<dsTSTRING<char>,8>::Common<dsCMP>) — set intersection of two
    // CMP-ascending-sorted vectors: append every element of `arr2` whose CMP key also appears in
    // `arr1` (sorted-merge two-pointer scan; ties advance both, taking `arr2`'s copy).
    template<class Cmp>
    void Common(const dsVECTOR<T, N> &arr1, const dsVECTOR<T, N> &arr2, const Cmp &cmp);

    // 0x8263ABB8 (dsVECTOR<dsTSTRING<char>,8>::Difference<dsCMP>) — symmetric set difference of
    // two CMP-ascending-sorted vectors: append every element present in exactly one of `arr1`/`arr2`
    // (sorted-merge two-pointer scan; ties advance both without appending, leftovers all appended).
    template<class Cmp>
    void Difference(const dsVECTOR<T, N> &arr1, const dsVECTOR<T, N> &arr2, const Cmp &cmp);

protected:
    // 0x82519D58 (dsVECTOR<psSECTION_RECORD,8>) / 0x82A788E0 (dsVECTOR<AFFIX,8>) — heap-sort fallback
    // used by Sort for large vectors. Builds a max-heap over the 1-based array (via Sisp sift-downs),
    // repeatedly swaps the root to the shrinking tail, then runs a verify-and-selection-sort cleanup
    // pass. Protected (IAA mangle). Reversed per-instantiation.
    template<class Cmp, class Swap>
    void HeapSort(const Cmp &cmp, const Swap &swap);
    // 0x825192F8 (dsVECTOR<psSECTION_RECORD,8>) — heap sift-down helper backing HeapSort: restore
    // the max-heap property over array[p..q] by comparing (via `cmp`) each node's key against the
    // larger of its two children and sinking the root's value into place. `swap` is the stateless
    // exchange policy (the actual element move is inlined). Reversed per-instantiation.
    template<class Cmp, class Swap>
    void Sisp(int p, int q, T *array, const Cmp &cmp, const Swap &swap);

    // 0x82508360 etc. / 0x82681440 (dsVECTOR<mtlMTL,8>) / 0x825166E8 (dsVECTOR<psSECTION,8>) —
    // resize the backing store to `size` elements via the debug reallocator. For pointer/4-byte
    // -handle element types this is a bare dlRealloc (trivially relocatable).
    void _internalRealloc(int size);

    // 0x823F0790 etc. / 0x826810D0 (dsVECTOR<mtlMTL,8>) / 0x82516690 (dsVECTOR<psSECTION,8>) —
    // free a backing-storage block (protected helper used by the destructor; for pointer/handle
    // element types this is a bare dlFree with no per-element teardown).
    void _internalFree(void *p);
};
