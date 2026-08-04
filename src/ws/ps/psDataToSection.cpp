#include "psSECTION.h"
#include "../ds/dsDATA.h"
#include "../ds/ds_boundary.h"

// psDataToSection @ 0x825B0F38
// Extract `obj`'s boxed value as a psSECTION, but ONLY on an exact type match (unlike
// dsDATA::GetValue<psSECTION>, this does not dispatch the runtime converter for other stored
// types). On a type mismatch (including empty), fails (0). On a match, copy-constructs a scratch
// psSECTION from `obj` (bumping the section's refcount), assigns it out to `val`, then releases
// the scratch.
int psDataToSection(const dsDATA &obj, psSECTION *val)
{
    // mangled ?psDataToSection@@YAHABVdsDATA@@PAVpsSECTION@@@Z: arg0 is const dsDATA& (reference).
    if (obj.type != dsDATA_TYPE_STORAGE<psSECTION>::dataType)
        return 0;

    psSECTION scratch(obj);
    *val = scratch;
    return 1;
}
