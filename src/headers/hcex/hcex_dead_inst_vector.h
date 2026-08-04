#pragma once
/* hcex_dead_inst_vector — dsVECTOR<animINST*,8>: scene instances queued for deferred destruction. */

#include "../animINST.h"

/* DB dsVECTOR<animINST *,8> via the canonical template (the former flat model had pData/nElem
 * swapped relative to the DB layout: pData@0, nElem@4). */
#include "ws/ds/dsVECTOR.h"
typedef dsVECTOR<animINST *, 8> hcex_dead_inst_vector;
