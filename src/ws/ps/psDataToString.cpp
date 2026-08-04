#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsCONVERTION_TYPE.h"

// psDataToString @ 0x825B2478
// Thin free-function wrapper: retrieve `obj`'s boxed value as a dsTSTRING<char> via the
// GetValue<dsTSTRING<char>> converter/dispatch path, using the "retrieve" conversion direction.
int psDataToString(const dsDATA &obj, dsTSTRING<char> *val)
{
    // mangled ?psDataToString@@YAHABVdsDATA@@PAV?$dsTSTRING@D@@@Z: arg0 is const dsDATA& (reference).
    return obj.GetValue<dsTSTRING<char> >(*val, DSD_CONV_RETRIEVE);
}
