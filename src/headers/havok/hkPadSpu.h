#pragma once
/* hkPadSpu<T> — SPU-padding wrapper; on non-SPU targets it is just the wrapped
   scalar/pointer (DB: hkPadSpu<char *> size 4, single member m_storage). */
template<class T>
struct hkPadSpu
{
    T m_storage; /* 0x00 */
};
