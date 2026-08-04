#pragma once
/* hcex_object_vector — dsVECTOR<HCEX_OBJ,8> viewed through just the leading id field (partial: callers
 * here only read pData[0].id). The full element layout is in HCEX_OBJ.h. */

/* DB dsVECTOR<HCEX_OBJ,8> via the canonical template. The former partial hcex_object view
 * (id-only, 4-byte stride) and the flat vector (pData/nElem swapped vs the DB pData@0/nElem@4)
 * are superseded; HCEX_OBJ.id is at offset 0, so pData[i].id reads are unchanged for i==0 and
 * now use the correct 56-byte element stride for i>0. */
#include "HCEX_OBJ.h"
#include "../ws/ds/dsVECTOR.h"
typedef HCEX_OBJ hcex_object;
typedef dsVECTOR<HCEX_OBJ, 8> hcex_object_vector;
