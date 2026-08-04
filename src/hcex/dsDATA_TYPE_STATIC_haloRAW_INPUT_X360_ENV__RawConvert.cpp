#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::RawConvert @ 0x823BDDC8
// As Convert, but over raw (untyped) storage pointers.
template<>
int ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::RawConvert(
        const void *src, void *dest, const dsDATA_TYPE *type, dsCONVERTION_TYPE convType)
{
    int srcTypeId = type->GetTypeId();
    int (*rawConverter)(const void *, void *) =
        ds_data::GetRawConverter(0, srcTypeId, convType);
    if (rawConverter)
        return rawConverter(src, dest);
    return 0;
}
