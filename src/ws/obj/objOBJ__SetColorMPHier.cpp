/* ?SetColorMPHier@objOBJ@@QAAXK@Z @0x826838C0 */
#include "objOBJ.h"
// 0x826838C0  ?SetColorMPHier@objOBJ@@QAAXK@Z
// Recursively apply a multiplayer tint colour down the child hierarchy, then set it on this
// object's own geometry and mark its render state dirty (clears the 0x100 stateRend bit).

void objOBJ::SetColorMPHier(unsigned int color)
{
    objOBJ *firstChild = nullptr;
    int started = 0;
    objOBJ *iter = child; // `this` is non-null on the reversed entry path
    firstChild = iter;

    while (iter && (!started || iter != firstChild)) {
        iter->SetColorMPHier(color);
        started = 1;
        iter = iter->next;
    }

    objGEOM_UNSHARED *geom = pGeom;
    if (geom) {
        geom->color_mp = color;
        stateRend &= ~0x100u;
    }
}
