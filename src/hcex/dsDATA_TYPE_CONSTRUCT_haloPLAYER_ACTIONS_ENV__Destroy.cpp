#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include <new>

// ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Destroy() @ 0x823BBFA0
// The value is boxed on the heap (the storage cell holds the pointer): free it and null the cell.
// DEVIATION: the cell was spelled `void **` here; the DB mangling is `SAXPAK@Z` (`unsigned long *`)
// and the canonical ws/ds/dsDATA_TYPE_CONSTRUCT.h now declares the one 4-byte-cell spelling shared
// with the haloRAW_INPUT_X360_ENV twin at 0x823BCE00.
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Destroy(unsigned int *storage)
{
    ::operator delete(reinterpret_cast<void *>(*storage));
    *storage = 0;
}
