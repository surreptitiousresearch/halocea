#pragma once
/* Generic hkArray<T> layout (12 bytes) — templated to carry the DB's element type
   (hkArray<int>, hkArray<hkVector4>, ...). T defaults to void so legacy call sites that
   treated the array generically can spell hkArray<> (raw void* data pointer, identical
   layout/semantics). */
template<class T = void>
struct hkArray
{
    T  *m_data;             /* 0x00 */
    int m_size;             /* 0x04 */
    int m_capacityAndFlags; /* 0x08 */
};
