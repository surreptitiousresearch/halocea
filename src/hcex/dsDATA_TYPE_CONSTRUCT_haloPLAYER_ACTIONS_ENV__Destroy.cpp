#include "../headers/ws/ds/dsDATA_TYPE_HELPERS.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include <new>

// ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Destroy() @ 0x823BBFA0
// The value is boxed on the heap (the storage cell holds the pointer): free it and null the cell.
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Destroy(void **storage)
{
    ::operator delete(*storage);
    *storage = nullptr;
}
