#pragma once
/* hcex_surf_vector — dsVECTOR<hcexBREAKABLE_SURF,8>: breakable-surface bridge slots. */

#include "hcex_breakable_surf.h"

/* DB dsVECTOR<hcexBREAKABLE_SURF,8> via the canonical template (the former flat model had
 * pData/nElem swapped relative to the DB layout: pData@0, nElem@4). */
#include "../ws/ds/dsVECTOR.h"
typedef dsVECTOR<hcexBREAKABLE_SURF, 8> hcex_surf_vector;
