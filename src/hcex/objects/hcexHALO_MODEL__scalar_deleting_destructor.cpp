/* hcexHALO_MODEL::`scalar deleting destructor' @0x823D342C — reset __vftable to the
 * entMANAGER_HANDLER base slot, then conditionally operator-deletes `this` when bit 0 of
 * `deleteFlags` is set (compiler-generated `??_G` thunk). */

#include "../../headers/hcex/hcexHALO_MODEL.h"

hcexHALO_MODEL *hcexHALO_MODEL::scalarDeletingDtor(unsigned char deleteFlags)
{
    hcexHALO_MODEL *self = this;
    __vftable = &entMANAGER_HANDLER::vftable;
    if ( deleteFlags & 1 )
    {
        operator delete(this);
        return self;
    }
    return this;
}
