#pragma once
/* fp_model_vector — dsVECTOR<FP_MODEL,8>: first-person bridge models. */

#include "animINST.h"
#include "hcex/FP_MODEL.h"   /* canonical FP_MODEL body (24 bytes, types_members-verified) */

/* DB dsVECTOR<FP_MODEL,8> via the canonical template (the former flat model had pData/nElem
 * swapped relative to the DB layout: pData@0, nElem@4). */
#include "ws/ds/dsVECTOR.h"
typedef dsVECTOR<FP_MODEL, 8> fp_model_vector;
