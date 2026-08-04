#include "../../headers/ws/txm/txmMANAGER.h"

// 0x826F6070 -- ?GetSizeY@txmTEXTURE@@QBAHXZ
// Logical height. Mirror of GetSizeX: when split-screen scaling is active (txmManager state bit
// 0x8000), follow the pSplitScreenTex[0] chain to the leaf sub-texture and report its height.
int txmTEXTURE::GetSizeY() const
{
    const txmTEXTURE *cur = this;
    for (txmTEXTURE *next = cur->pSplitScreenTex[0]; next; next = next->pSplitScreenTex[0])
    {
        if (((txmManager->state.state >> 15) & 1) == 0)
            break;
        cur = next;
    }
    return cur->sizeY;
}
