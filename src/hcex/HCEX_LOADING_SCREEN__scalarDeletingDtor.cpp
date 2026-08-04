/* HCEX_LOADING_SCREEN::`scalar deleting destructor' @0x823F41D8 — run the dtor chain, then
 * `operator delete` this object if the low bit of `freeMemory` is set. */

#include "../headers/hcex/HCEX_LOADING_SCREEN.h"

extern void operator_delete(void *block);

HCEX_LOADING_SCREEN *HCEX_LOADING_SCREEN::ScalarDeletingDestructor(unsigned int freeMemory)
{
    this->~HCEX_LOADING_SCREEN();

    if (freeMemory & 1)
        operator_delete(this);

    return this;
}
