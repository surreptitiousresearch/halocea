#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Copy @ 0x823BDC88
// Thin forwarder to the construct policy's Copy (copy stored-value slot to slot).
template<>
void ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Copy(const unsigned int *src, unsigned int *dest)
{
    ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Copy(src, dest);
}
