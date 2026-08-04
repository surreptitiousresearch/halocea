#include "objOBJ.h"
#include "obj_boundary.h"
// 0x82A7E5A8  ?StdAllocObjData@objOBJ@@SAPAV1@... (SAPAX) — static object-data allocator.
// For the aligned-data class (data == 35) it frees any old aligned block and returns a fresh
// 16-byte-aligned allocation; every other class routes through the plain realloc allocator.

void *objOBJ::StdAllocObjData(objOBJ *pObj, int data, unsigned int size, void *oldData)
{
    (void)pObj;
    if (data != 35)
        return dlRealloc(oldData, size,
                         "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0x50D);

    if (oldData)
        dlFreeAligned(oldData);
    return dlMemalign(size, 0x10,
                      "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0x50A);
}
