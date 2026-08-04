#pragma once
/* hkFlags<BITS, STORAGE> — Havok flags wrapper stored in an explicit-width
   integer. Layout is just the storage scalar (mirrors hkEnum). */
template<class BITS, class STORAGE>
struct hkFlags
{
    STORAGE m_storage; /* 0x00 */
};
