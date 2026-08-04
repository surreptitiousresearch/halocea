#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Convert @ 0x823BEC18
// Virtual override. Look up a converter (dstTypeId 0 = this non-primitive type) keyed by
// srcType->GetTypeId() and the conversion direction; run it, or fail (0) when none registered.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Convert(
        const unsigned int *src, unsigned int *dest, const dsDATA_TYPE *type, dsCONVERTION_TYPE convType) const
{
    int srcTypeId = type->GetTypeId();
    int (*converter)(const unsigned int *, unsigned int *) =
        ds_data::GetConverter(0, srcTypeId, convType);
    if (converter)
        return converter(src, dest);
    return 0;
}
