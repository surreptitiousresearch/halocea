/* ?GetSizeX@txmTEXTURE@@QBAHXZ @0x826F6030 */
#include "../../headers/ws/txm/txmMANAGER.h"

// 0x826F6030 -- ?GetSizeX@txmTEXTURE@@QBAHXZ
// Logical width. When split-screen scaling is active (txmManager state bit 0x8000), follow the
// pSplitScreenTex[0] chain to the leaf sub-texture and report its width; otherwise report this
// texture's own width.
int txmTEXTURE::GetSizeX() const
{
    const txmTEXTURE *cur = this;
    for (txmTEXTURE *next = cur->pSplitScreenTex[0]; next; next = next->pSplitScreenTex[0])
    {
        if (((txmManager->state.state >> 15) & 1) == 0)
            break;
        cur = next;
    }
    return cur->sizeX;
}
