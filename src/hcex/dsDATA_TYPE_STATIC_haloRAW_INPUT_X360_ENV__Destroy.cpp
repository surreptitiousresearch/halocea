#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Destroy @ 0x823BDC60
// Thin forwarder to the construct policy's Destroy. Storage is the 4-byte inline slot (PAK).
template<>
void ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Destroy(unsigned int *storage)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Destroy(storage);
}
