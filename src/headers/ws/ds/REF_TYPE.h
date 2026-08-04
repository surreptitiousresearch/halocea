#pragma once
// ws-engine ds_data: typed reference-counted handle used by the dsDATA_TYPE_IMPL
// property/dispatch system (e.g. gsUSER_INPUT_MNG's context properties).
// DB-verified layout (types_members ds_data::REF_TYPE<gsiCONTEXT>): m_pData@0 — size 4.

namespace ds_data {

// Ref-count/ownership policy tag for REF_TYPE_DATA. Enumerators per headers_ref
// (the DB carries the enum as a member-less shell).
enum OBJECT_POLICY : int {
    opRefCount   = 0,
    opExternal   = 1,
    opROExternal = 2,
};

// Backing ref-counted storage cell for REF_TYPE<T>.
// DB-verified layout (types_members ds_data::REF_TYPE_DATA<dsVECTOR_PARAM_LIST>): m_pT@0 (T*),
// m_iRefCount@4 (int), m_op@8 (ds_data::OBJECT_POLICY) — size 12.
template<class T>
struct REF_TYPE_DATA {
    T             *m_pT;        // 0x00 the referenced value
    int            m_iRefCount; // 0x04
    OBJECT_POLICY  m_op;        // 0x08

    // Default ctor: allocate + default-construct a fresh T as the owned value, adopt the
    // ref-counting ownership policy, and start the refcount at 1.
    // e.g. <dsVECTOR_PARAM_LIST> @ 0x823D3A68.
    REF_TYPE_DATA();
};

template<class T>
struct REF_TYPE {
    REF_TYPE_DATA<T> *m_pData; // 0x00 ref-counted backing storage (null = empty handle)

    // Construction/destruction/accessors — bodies external to this batch.
    REF_TYPE();                          // boundary
    ~REF_TYPE();                         // boundary
    T *operator->() const;               // boundary

    // Drop this handle's reference to `data` (releasing/destroying the backing REF_TYPE_DATA<T> at
    // zero refcount). Seen at multiple call sites as `Release(&ref, ref.m_pData)` — the decompiler's
    // `ref.__vftable`/other-field aliases for the second argument are stack-slot-reuse noise (this
    // template has no vtable); the real second argument is always this handle's own m_pData.
    // Per-instantiation address not looked up in this batch; body not decompiled. // boundary
    void Release(REF_TYPE_DATA<T> *data);
};

} // namespace ds_data
