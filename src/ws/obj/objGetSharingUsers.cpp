#include "objOBJ.h"
#include "objDEPENDENCY_CACHE.h"

// objGetSharingUsers(objOBJ*,dsVECTOR<objOBJ*,8>&,bool) @ 0x8268BE48
// Clear `users`, then — only if `obj` has the OBJ_ST_GEOM_SHARED-family state bit (0x10) set —
// (re)build the shared global dependency cache for `obj`'s scene tree and collect either the
// full transitive closure of sharing users (getAllDependentUsers) or just the direct ones.
void objGetSharingUsers(objOBJ *obj, dsVECTOR<objOBJ *, 8> &users, bool getAllDependentUsers)
{
    users.nElem = 0;

    if (obj->state & 0x10) {
        objDependencyCache.Init(obj);
        if (getAllDependentUsers)
            objDependencyCache.GetAllDependentUsers(obj, &users);
        else
            objDependencyCache.AddSharingUsers(obj, &users);
    }
}
