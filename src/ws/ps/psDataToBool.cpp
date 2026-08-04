#include "../ds/dsDATA.h"
#include "../ds/dsCONVERTION_TYPE.h"
// 0x825B0EEC — retrieve `obj`'s value as a bool (explicit conversion), then widen it into `*val`;
// returns whether the retrieval succeeded.

int psDataToBool(const dsDATA *obj, int *val)
{
    bool b;
    if (!obj->GetValue<bool>(b, DSD_CONV_EXPLICIT))
        return 0;
    *val = b;
    return 1;
}
