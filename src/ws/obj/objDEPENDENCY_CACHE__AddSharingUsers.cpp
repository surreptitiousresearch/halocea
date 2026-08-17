/* ?AddSharingUsers@objDEPENDENCY_CACHE@@QBAHPAVobjOBJ@@AAV?$dsVECTOR@PAVobjOBJ@@$07@@@Z @0x82689CC0 */
#include "objDEPENDENCY_CACHE.h"
#include "objOBJ.h"
// 0x82689CC0  ?AddSharingUsers@objDEPENDENCY_CACHE@@QBAHPAVobjOBJ@@AAV?$dsVECTOR@PAVobjOBJ@@$07@@@Z
// Append `obj`'s direct sharing users onto `users`; returns how many were added (0 if `obj`
// has no dependency-map entry). If this cache is mid-rebuild it defers to the shared global
// instance, spinning until that build settles.
//
// CAVEAT: const (QBA) despite the phantom trailing register args the decompiler invented. The
// dependency map (offset 4) is modeled as a boundary ds::MAP; its lookup/append helpers are
// declared in objDEPENDENCY_CACHE.h and not reversed here.

int objDEPENDENCY_CACHE::AddSharingUsers(objOBJ *obj, dsVECTOR<objOBJ *, 8> *users) const
{
    typedef ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
        DependencyMap;
    typedef ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP,
                                         dsNODE_CACHE_ALLOCATOR> DependencyMapConstIter;

    const objDEPENDENCY_CACHE *cache = this;
    if (isCached) {
        cache = &objDependencyCache;
        while (objDependencyCache.isCached)
            ; // wait for the shared cache build to finish
    }

    objOBJ *key = obj;
    const DependencyMap *map = reinterpret_cast<const DependencyMap *>(&cache->dependencies);
    DependencyMapConstIter found = map->Find(&key);
    if (!found.item)
        return 0;

    const dsVECTOR<objOBJ *, 8> *sharingUsers = &*found; // obj's direct dependents
    ds::AppendFast<objOBJ *>(users, sharingUsers, 0, -1);
    return found->nElem;
}
