#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::RawConvert @ 0x823BF228
// Unboxed-value conversion: look up a raw converter (dstTypeId 0) for src->GetTypeId(); 0 if none.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::RawConvert(
        const void *src, void *dest,
        const dsDATA_TYPE *type, dsCONVERTION_TYPE convType) const
{
    int typeId = type->GetTypeId();
    int (*rawConverter)(const void *, void *) =
        ds_data::GetRawConverter(0, typeId, convType);
    if (rawConverter)
        return rawConverter(src, dest);
    return 0;
}
