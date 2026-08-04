#include "objOBJ.h"
#include "obj_boundary.h"
// 0x82A7EB40  ?Delete@objOBJ@@SAXPAV1@@Z
// Static destroy-and-free helper (matches Make's dlMemalign allocation via dlFreeAligned).

void objOBJ::Delete(objOBJ *obj)
{
    if (obj)
    {
        obj->~objOBJ();
        dlFreeAligned(obj);
    }
}
