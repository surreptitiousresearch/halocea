#include "psARRAY.h"
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/ds_boundary.h"
#include <string.h>

// psDataToArray @ 0x825B4E40
// Extract `obj`'s boxed value as a psARRAY. On an exact type match, copy-constructs a scratch
// psARRAY from `obj` (bumping the array's refcount) and assigns it out to `val`. Otherwise, unless
// `obj` is empty (fails, 0), boxes `obj` itself as the sole element of a fresh single-element
// dsVECTOR<dsDATA,8> and installs that as `val`'s contents via SetElements — i.e. a non-array value
// gets wrapped as a 1-element array. `__cl` is the debug allocation call-site cookie recorded for
// the scratch vector (source line from the original ps_sys.cpp).
int psDataToArray(const dsDATA &obj, psARRAY *val)
{
    // mangled ?psDataToArray@@YAHABVdsDATA@@PAVpsARRAY@@@Z: arg0 is const dsDATA& (reference).
    const dsDATA_TYPE *type = obj.type;
    if (type == dsDATA_TYPE_STORAGE<psARRAY>::dataType) {
        psARRAY scratch(obj);
        *val = scratch;
    } else {
        if (!type)
            return 0;

        dsVECTOR<dsDATA, 8> singleElement;
        memset(&singleElement, 0, 12); // zero pData/nElem/allocated; __cl set explicitly below
        singleElement.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ps\\ps_sys.cpp";
        singleElement.__cl.line = 667;
        singleElement.PushBack(obj);
        val->SetElements(singleElement);
    }
    return 1;
}
