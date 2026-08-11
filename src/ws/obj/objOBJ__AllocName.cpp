#include "objOBJ.h"
#include "obj_boundary.h"
// @0x82A7E3A8  ?_AllocName@objOBJ@@QAAHH@Z
// (Re)allocate the name buffer to nameLen+1 bytes and NUL-terminate it.
// Returns 1 on success (or when a zero-length request needs no buffer), 0 on allocation failure.

int objOBJ::_AllocName(int nameLen)
{
    char *buffer = (char *)dlRealloc(
        name, nameLen + 1,
        "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0xCB);
    if (buffer)
    {
        buffer[0] = 0;
    }
    else if (nameLen > 0)
    {
        return 0; // real allocation failure
    }
    name = buffer;
    return 1;
}
