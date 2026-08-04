#pragma once
/* Generic hkObjectArray<T> layout (12 bytes) — same {data, size, capacityAndFlags}
   triple as hkArray<T>, but owns/constructs its elements. Templated to carry the
   DB element type; T defaults to void for generic call sites. */
template<class T = void>
struct hkObjectArray
{
    T  *m_data;             /* 0x00 */
    int m_size;             /* 0x04 */
    int m_capacityAndFlags; /* 0x08 — sign bit = DONT_DEALLOCATE */
};
