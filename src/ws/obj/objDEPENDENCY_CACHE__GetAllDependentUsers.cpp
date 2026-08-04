#include "objDEPENDENCY_CACHE.h"
#include "objOBJ.h"
// 0x82689D80  ?GetAllDependentUsers@objDEPENDENCY_CACHE@@QBAXPAVobjOBJ@@AAV?$dsVECTOR@...@Z
// Append the transitive closure of objects that share geometry from `obj` into `users`: gather
// `obj`'s direct sharing users, then recurse into each newly-added user. If this cache is still
// being (re)built, defer to the shared global instance (spin until it settles).

void objDEPENDENCY_CACHE::GetAllDependentUsers(objOBJ *obj, dsVECTOR<objOBJ *, 8> *users) const
{
    objDEPENDENCY_CACHE *cache = const_cast<objDEPENDENCY_CACHE *>(this);
    if (isCached) {
        cache = &objDependencyCache;
        while (objDependencyCache.isCached)
            ; // wait for the shared cache build to finish
    }

    int firstNew = users->nElem;
    int addedCount = cache->AddSharingUsers(obj, users);
    for (int i = 0; i < addedCount; ++i) {
        objOBJ **user = &(*users)[firstNew + i];
        cache->GetAllDependentUsers(*user, users);
    }
}
