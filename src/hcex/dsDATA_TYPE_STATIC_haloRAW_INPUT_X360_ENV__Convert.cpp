#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Convert @ 0x823BDD38
// Look up a converter (dstTypeId 0 = this non-primitive type) from srcType->GetTypeId() and run it.
template<>
int ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::Convert(
        const unsigned int *src, unsigned int *dest, const dsDATA_TYPE *type, dsCONVERTION_TYPE convType)
{
    int srcTypeId = type->GetTypeId();
    int (*converter)(const unsigned int *, unsigned int *) =
        ds_data::GetConverter(0, srcTypeId, convType);
    if (converter)
        return converter(src, dest);
    return 0;
}
