#pragma once
/* hkRefPtr<T> — Havok reference-counted smart pointer (4 bytes). DB-verified layout
   (types_members hkRefPtr<hkdGraphicsShape>): m_pntr@0. */
template<class T>
struct hkRefPtr
{
    T *m_pntr; /* 0x00 */
    /* DB-implied smart-pointer conversions (layout-neutral, non-virtual inline). */
    operator T*() const { return m_pntr; }
    T *operator->() const { return m_pntr; }
    T *val() const { return m_pntr; }
};
