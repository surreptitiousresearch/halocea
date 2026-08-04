#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::RawConvert @ 0x823BECB8
// Virtual override, unboxed-storage twin of Convert. Look up a raw converter (dstTypeId 0) keyed
// by srcType->GetTypeId() and the direction; run it, or fail (0) when none registered.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::RawConvert(
        const void *src, void *dest, const dsDATA_TYPE *type, dsCONVERTION_TYPE convType) const
{
    int srcTypeId = type->GetTypeId();
    int (*rawConverter)(const void *, void *) =
        ds_data::GetRawConverter(0, srcTypeId, convType);
    if (rawConverter)
        return rawConverter(src, dest);
    return 0;
}
