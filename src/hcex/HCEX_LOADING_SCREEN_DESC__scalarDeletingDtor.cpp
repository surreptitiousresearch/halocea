/* HCEX_LOADING_SCREEN_DESC::`scalar deleting destructor' @0x823F4178 — run ~sslCLASS_REF then
 * ~dscDESC (implicitly, via the base destructor chain), then `operator delete` this object if
 * the low bit of `freeMemory` is set. */

#include "../headers/hcex/HCEX_LOADING_SCREEN_DESC.h"

extern void operator_delete(void *block);

HCEX_LOADING_SCREEN_DESC *HCEX_LOADING_SCREEN_DESC::ScalarDeletingDestructor(unsigned int freeMemory)
{
    sslClass.~sslCLASS_REF();

    if (freeMemory & 1)
        operator_delete(this);

    return this;
}
