/* ?SetRenderPassId@objOBJ@@QAAXH@Z @0x826823C0 */
#include "objOBJ.h"
// 0x826823C0  ?SetRenderPassId@objOBJ@@QAAXH@Z
// Set the render-pass id on this object's geometry and recurse over the whole child sibling
// ring. The ring is walked with a first-iteration guard so the circular `next` list terminates.

void objOBJ::SetRenderPassId(int passId)
{
    if (!this)
        return;

    objOBJ *firstChild = child;
    bool walked = false;

    if (pGeom)
        pGeom->renderPassId = (char)passId;

    for (objOBJ *node = firstChild; firstChild && !(walked && node == firstChild); node = node->next)
    {
        node->SetRenderPassId(passId);
        walked = true;
    }
}
