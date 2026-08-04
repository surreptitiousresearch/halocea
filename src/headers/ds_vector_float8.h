#pragma once
/* dsVECTOR<float,8> — ws-engine small-buffer float vector. Now an alias of the canonical
 * dsVECTOR<T,N> template (DB-verified: pData@0, nElem@4, allocated@8, __cl@12). */

#include "ws/ds/dsVECTOR.h"

typedef dsVECTOR<float, 8> dsVECTOR_float_8;
