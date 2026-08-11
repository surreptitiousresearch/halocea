/* hcexPARTICLE_SYS::`vector deleting destructor' @0x823D1570 — reset __vftable to the
 * gsPARTICLE_SYS base slot, then conditionally operator-deletes `this` when bit 0 of
 * `deleteFlags` is set (compiler-generated `??_E` thunk). */

#include "../../headers/hcex/hcexPARTICLE_SYS.h"

hcexPARTICLE_SYS *hcexPARTICLE_SYS::vectorDeletingDtor(unsigned char deleteFlags)
{
    hcexPARTICLE_SYS *self = this;
    __vftable = &gsPARTICLE_SYS::vftable;
    if ( deleteFlags & 1 )
    {
        operator delete(this);
        return self;
    }
    return this;
}
