/* hcexHALO_CUSTOM_OBJ_LIST::`vector deleting destructor' @0x823DCB88 — same teardown as the real
 * destructor, then conditionally operator-deletes `self` when bit 0 of `deleteFlags` is set
 * (compiler-generated `??_E` thunk). */

#include "../../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"

hcexHALO_CUSTOM_OBJ_LIST *hcexHALO_CUSTOM_OBJ_LIST::vectorDeletingDtor(unsigned char deleteFlags)
{
    cdList.~dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>();
    __vftable = reinterpret_cast<instMANAGER_HANDLER_vtbl *>(&instMANAGER_HANDLER::vftable); // base __vftable type
    if ( deleteFlags & 1 )
        operator delete(this);
    return this;
}
