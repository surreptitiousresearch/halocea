#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Destroy @ 0x823BCE00
// Free the heap-boxed value the storage slot points at, then null the slot.
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Destroy(unsigned int *storage)
{
    ::operator delete(reinterpret_cast<void *>(*storage));
    *storage = 0;
}
