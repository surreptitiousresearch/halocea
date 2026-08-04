/* hcexHALO_CUSTOM_OBJ_LIST::~hcexHALO_CUSTOM_OBJ_LIST @0x823D97F0 — tear down cdList (its
 * templated dtor is boundary/not reversed in this batch), then reset __vftable to the
 * instMANAGER_HANDLER base slot. */

#include "../../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"

hcexHALO_CUSTOM_OBJ_LIST::~hcexHALO_CUSTOM_OBJ_LIST()
{
    cdList.~dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>(); // match cdList's member type (unsigned long key)
    __vftable = &instMANAGER_HANDLER::vftable; // base __vftable is instMANAGER_HANDLER_vtbl*
}
