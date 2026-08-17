/* ?GetRTSizeY@txmTEXTURE@@QBAHXZ @0x826F60F8 */
#include "headers/ws/txm/txmTEXTURE.h"
#include "headers/ws/txm/txmMANAGER.h"

// 0x826F60FC
int txmTEXTURE::GetRTSizeY() const
{
    if (this->rtHeight != 0)
        return this->rtHeight;

    txmTEXTURE *primaryViewportTex = this->pSplitScreenTex[0];
    if (primaryViewportTex && ((txmManager->state.state >> 15) & 1) != 0)
        return primaryViewportTex->GetSizeY();

    return this->sizeY;
}
