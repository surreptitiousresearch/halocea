#pragma once
/* Generic hkInplaceArray<T,N> — hkArray<T> with N inline elements
   (DB: hkInplaceArray<hkpEntity *,1> size 16). */
#include "hkArray.h"

template<class T, int N>
struct hkInplaceArray : hkArray<T>
{
    T m_storage[N]; /* 0xC */
};
