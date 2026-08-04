#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Convert @ 0x823BF188
// Look up a boxed converter (dstTypeId 0) for src->GetTypeId() and run it; 0 if none.
template<>
int ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Convert(
        const unsigned int *src, unsigned int *dest,
        const dsDATA_TYPE *type, dsCONVERTION_TYPE convType) const
{
    int typeId = type->GetTypeId();
    int (*converter)(const unsigned int *, unsigned int *) =
        ds_data::GetConverter(0, typeId, convType);
    if (converter)
        return converter(src, dest);
    return 0;
}
