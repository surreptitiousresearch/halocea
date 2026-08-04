#pragma once
#include "../../apCL.h"
// ws-engine ds: global-namespace growable array `dsARRAY<T>` (distinct from ds::ARRAY in
// dsARRAY.h, which is the fixed inline array). DB-verified layout
// (types_members dsARRAY<animSEQ::AF> / dsARRAY<float>): pData@0, nElem@4, __cl@8 — size 16.

template<class T>
struct dsARRAY {
    T   *pData; // 0x00 heap element array
    int  nElem; // 0x04 number of live elements
    apCL __cl;  // 0x08 allocation call-site cookie {file, line}

    // Reallocate the backing store to hold `nElem` elements (0 frees it). DB
    // ?Realloc@?$dsARRAY@K@@IAAXH@Z (protected in origin; declared public here to match the
    // dsVECTOR::Realloc convention and permit cross-class callers). boundary (2026-07-31)
    void Realloc(int nElem);
};
