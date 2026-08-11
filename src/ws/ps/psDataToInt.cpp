#include "../ds/dsDATA.h"
#include "../ds/ds_boundary.h"
#include "../ds/dsCONVERTION_TYPE.h"
// @0x825B0EA0 — retrieve `obj`'s value as an int. When the stored value is a float, an explicit
// (narrowing) conversion is used; otherwise the normal retrieve-conversion path is used (matching
// dsDATA::GetValue<int>'s int<-various converter table).

int psDataToInt(const dsDATA *obj, int *val)
{
    if (obj->type == dsDATA_TYPE_STORAGE<float>::dataType)
        return obj->GetValue<int>(*val, DSD_CONV_EXPLICIT);
    return obj->GetValue<int>(*val, DSD_CONV_RETRIEVE);
}
