/* hcexHALO_OBJECT::`scalar deleting destructor' @0x823D1298 — reset __vftable to the
 * entMANAGER_HANDLER base slot, then conditionally operator-deletes `this` when bit 0 of
 * `deleteFlags` is set (compiler-generated `??_G` thunk). */

#include "../../headers/hcex/hcexHALO_OBJECT.h"

hcexHALO_OBJECT *hcexHALO_OBJECT::scalarDeletingDtor(unsigned char deleteFlags)
{
    hcexHALO_OBJECT *self = this;
    __vftable = &entMANAGER_HANDLER::vftable;
    if ( deleteFlags & 1 )
    {
        operator delete(this);
        return self;
    }
    return this;
}
