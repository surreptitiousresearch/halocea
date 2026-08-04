#include <new>
#include "objOBJ.h"
#include "obj_boundary.h"
// 0x82A7E300  ?Make@objOBJ@@SAPAV1@XZ
// Static factory: 16-byte-aligned allocation of one objOBJ (0xDC = 220 bytes = sizeof(objOBJ)),
// then placement-construct. Returns the new object (nullptr if the allocation failed).

objOBJ *objOBJ::Make()
{
    objOBJ *obj = (objOBJ *)dlMemalign(
        sizeof(objOBJ), 0x10,
        "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0xB1);
    if (obj)
        new (obj) objOBJ();
    return obj;
}
