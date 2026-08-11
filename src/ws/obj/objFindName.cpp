#include "objOBJ.h"
#include "objFILTER_NAME.h"
#include "obj_free_functions.h"
// @0x82682B58  ?objFindName@@YAPAVobjOBJ@@PAV1@PBD@Z
// Find the first node named `name` in the subtree rooted at `pObj` (self included).
// Builds an objFILTER_NAME predicate on the stack and defers to objFind(). Returns nullptr
// if `pObj` or `name` is null, or `name` is the empty string.

objOBJ *objFindName(objOBJ *pObj, const char *name)
{
    objFILTER_NAME filter(name);

    if (pObj && name && name[0] != '\0')
        return objFind(pObj, &filter);

    return nullptr;
}
