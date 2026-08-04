#pragma once
#include "../ds/dsCONST_ARRAY.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: fixed-capacity inline m3dV array (nEntry + m3dV[N]). DB-verified (headers_ref
// CONST_ARRAY_m3dV<10>) — thin derived alias over dsCONST_ARRAY<m3dV,N>, size 0x7C for N=10.

template<int N>
struct CONST_ARRAY_m3dV : dsCONST_ARRAY<m3dV, N> {
};
