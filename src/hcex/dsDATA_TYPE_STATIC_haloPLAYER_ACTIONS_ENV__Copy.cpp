#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::Copy @ 0x823BD7D0
// Thin forwarder to the construct policy's Copy (copy stored-value slot to slot).
template<>
void ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::Copy(const unsigned int *src, unsigned int *dest)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Copy(src, dest);
}
