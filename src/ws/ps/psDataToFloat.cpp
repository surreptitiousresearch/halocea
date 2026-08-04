#include "../ds/dsDATA.h"
#include "../ds/dsCONVERTION_TYPE.h"

// psDataToFloat @ 0x825B0EC8
// Thin free-function wrapper: retrieve `obj`'s boxed value as a float via the GetValue<float>
// converter/dispatch path, using the "retrieve" conversion direction.
int psDataToFloat(const dsDATA &obj, float *val)
{
    // mangled ?psDataToFloat@@YAHABVdsDATA@@PAM@Z: arg0 is const dsDATA& (decompiler lowered the
    // reference to a pointer); reference form matches the callers and the real symbol.
    return obj.GetValue<float>(*val, DSD_CONV_RETRIEVE);
}
