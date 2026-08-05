#pragma once
#include <stdint.h>
/* hkSmallArray<T> — Havok compact array (8 bytes). DB-verified layout
   (types_members hkSmallArray<hkdBreakableBodyListener *>): m_data@0, m_size@4,
   m_capacityAndFlags@6. */
template<class T>
struct hkSmallArray
{
    T *m_data;                          /* 0x00 */
    uint16_t m_size;            /* 0x04 */
    uint16_t m_capacityAndFlags;/* 0x06 */
};
