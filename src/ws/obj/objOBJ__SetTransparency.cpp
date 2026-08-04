#include "objOBJ.h"
// 0x82682110  ?SetTransparency@objOBJ@@QAAXE@Z
// Set geometry transparency (0..255 -> 0..1) on this object's own geometry, then recurse
// down the child hierarchy. The decompiler shows spurious trailing register args on both the
// entry signature and the recursive call; the real method takes a single byte argument.

void objOBJ::SetTransparency(unsigned char transp)
{
    objGEOM_UNSHARED *geom = pGeom;
    if (geom) {
        geom->transp = (float)transp * 0.0039215689f; // 1/255
    }

    objOBJ *firstChild = child;
    int started = 0;
    for (objOBJ *iter = firstChild; firstChild && (!started || iter != firstChild); iter = iter->next) {
        iter->SetTransparency(transp);
        started = 1;
    }
}
