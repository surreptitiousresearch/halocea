#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::Destroy @ 0x823BD7A8
// Thin forwarder to the construct policy's Destroy. Storage is the 4-byte inline slot (PAK).
template<>
void ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::Destroy(unsigned int *storage)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Destroy(storage);
}
