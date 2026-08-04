#pragma once
/* hcex_light_vector — dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8> viewed through just the leading key of each pair
 * (partial: hcex_obj_clear only reads pData[0].key to drive teardown). Full element in hcex_light.h. */

/* DB dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8> via the canonical templates (the former flat model had
 * pData/nElem swapped relative to the DB layout, and a 4-byte key-only element stride). */
#include "hcex_light.h"
typedef dsVECTOR<hcex_light_pair, 8> hcex_light_vector;
