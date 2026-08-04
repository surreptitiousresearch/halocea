#pragma once
#include "ds_assert_boundary.h"
// ws-engine ds: fixed-capacity vector with the element storage inlined (no heap).
// DB-verified layout (types_members dsSTATIC_VECTOR<dsSTRID,20>):
//   nElem@0, pData@4 (T[N]) — size 4 + N*sizeof(T).
// Template params: T = element type; N = fixed capacity.

template<class T, int N>
struct dsSTATIC_VECTOR {
    int nElem;    // 0x00 number of live elements
    T   pData[N]; // 0x04 inline storage

    // 0x825160D0 (dsSTATIC_VECTOR<dsSTRID,20>) — reference to the last live element
    // (asserts nElem>0).
    T &Back();

    // 0x82516148 (dsSTATIC_VECTOR<dsSTRID,20>) — append a copy of `val` into the inline
    // storage (asserts nElem < N first); returns a reference to the appended element.
    T &PushBack(const T &val);
};
