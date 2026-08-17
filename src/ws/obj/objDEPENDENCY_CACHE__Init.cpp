/* ?Init@objDEPENDENCY_CACHE@@QAAXPAVobjOBJ@@@Z @0x8268B9B0 */
#include "objDEPENDENCY_CACHE.h"
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objITER_TREE.h"
#include "obj_boundary.h"
#include "../ds/ds_assert_boundary.h"
// 0x8268B9B0  ?Init@objDEPENDENCY_CACHE@@QAAXPAVobjOBJ@@@Z
// Build this cache's dependency map for the scene tree containing `obj`.
//
// First climb from `obj` to the tree root, noting the lowest ancestor that is already a key in
// the map. If this cache already reports sharing users, or no ancestor was cached, do a full
// rebuild from the root. Otherwise probe incrementally downward from the cached ancestor: insert
// each visited node until one is found that has a geometry-sharing host (or the 0x10 state bit),
// which forces a full rebuild. A full rebuild clears the map, inserts every node in the tree,
// then walks the map registering each object that has a sharing host as a user of that host.
//
// CAVEAT: the backing container (offset 4) is the boundary ds::MAP hash map; only the DB-verified
// members Init touches (bucket vector `data`, node type ITEM {next,key,value}) and the helpers it
// calls (GetHash / Insert / Clear / Find) are modeled — see objDEPENDENCY_CACHE.h. The decompiler
// tangled MAP::Insert's sret NAMED_PAIR return and its empty value-vector temp into one stack
// blob; here Insert is modeled for its side effect (inserting `node` with an empty user list) and
// the temp's no-op teardown (dlFree(nullptr)) is the empty vector's destructor. The final
// population loop's opaque MAP iterator is reconstructed as direct bucket/chain traversal, which
// is exactly what the disassembly performs.

void objDEPENDENCY_CACHE::Init(objOBJ *obj)
{
    typedef ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
        DependencyMap;
    typedef DependencyMap::ITEM ItemNode;
    typedef ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP,
                                         dsNODE_CACHE_ALLOCATOR> DependencyMapConstIter;

    DependencyMap *map = reinterpret_cast<DependencyMap *>(&this->dependencies);

    // 1) Climb to the tree root; remember the lowest ancestor already present in the map.
    objOBJ *root = obj;
    objOBJ *cachedAncestor = nullptr;
    for (objOBJ *node = obj; node->parent; ) {
        node = node->parent;
        root = node;
        if (!cachedAncestor) {
            // Inlined map.Contains(node): hash to a bucket, scan the chain for a key match.
            objOBJ *key = node;
            bool present = false;
            int nBuckets = map->data.nElem;
            if (nBuckets) {
                int bucket = map->GetHash(&key) % nBuckets;
                if (map->data[bucket]) {
                    for (ItemNode *entry = map->data[bucket]; entry; entry = entry->next) {
                        if (entry->key == node) {
                            present = true;
                            break;
                        }
                    }
                }
            }
            if (present)
                cachedAncestor = node;
        }
    }

    // 2) Incremental probe from the cached ancestor (skipped straight to full rebuild when this
    //    cache already has sharing users, or when no ancestor was cached).
    if (!this->hasSharinUsers && cachedAncestor) {
        objITER_TREE iter(cachedAncestor);
        objOBJ *cur = cachedAncestor;
        bool sharingFound = false;
        for (;;) {
            objOBJ *key = cur;
            dsVECTOR<objOBJ *, 8> emptyUsers;
            map->Insert(&key, &emptyUsers); // insert cur with an empty user list

            objGEOM_UNSHARED *pGeom = cur->pGeom;
            if (pGeom && pGeom->pSharingHostObj) {
                sharingFound = true;
                break;
            }
            if (cur->state & 0x10) {
                sharingFound = true;
                break;
            }
            iter.Next();
            cur = iter.pCur;
            if (!cur)
                break; // probe completed with no sharing -> incremental result stands
        }
        if (sharingFound)
            this->hasSharinUsers = true;
    }

    if (!this->hasSharinUsers)
        return;

    // 3) Full rebuild: clear, insert every node from the root, then wire up sharing users.
    map->Clear();
    {
        objITER_TREE iter(root);
        objOBJ *node = root;
        do {
            objOBJ *key = node;
            dsVECTOR<objOBJ *, 8> emptyUsers;
            map->Insert(&key, &emptyUsers);
            iter.Next();
            node = iter.pCur;
        } while (node);
    }
    this->hasSharinUsers = false;

    // Walk every map entry; for each object that has a sharing host, append it to that host's
    // user list.
    int nBuckets = map->data.nElem;
    for (int bucket = 0; bucket < nBuckets; ++bucket) {
        for (ItemNode *entry = map->data[bucket]; entry; entry = entry->next) {
            objOBJ *key = entry->key;
            objGEOM_UNSHARED *pGeom = key->pGeom;
            objOBJ *hostObj = pGeom ? pGeom->pSharingHostObj : nullptr;
            if (hostObj) {
                DependencyMapConstIter hostInfo = map->Find(&hostObj);
                if (!IGNORE_STRONG_ASSERT && hostInfo.item == nullptr) {
                    static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                                        "!hostInfo.IsDone()",
                                        "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_misc.cpp",
                                        2120, empty_string);
                }
                ItemNode *hostNode = reinterpret_cast<ItemNode *>(hostInfo.item);
                hostNode->value.PushBack(key);
                this->hasSharinUsers = true;
            }
        }
    }
}
