#pragma once
/* Generic hkInplaceArrayAligned16<T,N> layout — an hkArray<T> whose initial
   storage is an inline 16-byte-aligned buffer of N elements (DB instantiations:
   hkInplaceArrayAligned16<BodyInfo::Normal,12> = 400,
   hkInplaceArrayAligned16<BodyInfo::Graphics,12> = 976,
   hkInplaceArrayAligned16<hkdShapeInstanceInfo,16> = 1296). */

#include "hkArray.h"

template<class T, int N>
struct hkInplaceArrayAligned16 : hkArray<T>
{
    int           m_padding;              /* 0x0C — aligns m_storage to 16 */
    unsigned char m_storage[N * sizeof(T)]; /* 0x10 — inline element buffer */
};
