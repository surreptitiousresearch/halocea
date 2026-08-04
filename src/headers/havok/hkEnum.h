#pragma once
/* hkEnum<ENUM, STORAGE> — Havok enum wrapper stored in an explicit-width integer.
   Layout is just the storage scalar. Templates cannot be spelled in C, so the C TUs
   substitute the storage scalar directly at each use site (see hkpShape.h) — same layout. */
#ifdef __cplusplus
template<class ENUM, class STORAGE>
struct hkEnum
{
    STORAGE m_storage; /* 0x00 */
};
#endif
