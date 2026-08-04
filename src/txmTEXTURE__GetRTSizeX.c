#include "headers/ws/txm/txmTEXTURE.h"
#include "headers/ws/txm/txmMANAGER.h"

// 0x826F60B4
int txmTEXTURE::GetRTSizeX() const
{
    if (this->rtWidth != 0)
        return this->rtWidth;

    txmTEXTURE *primaryViewportTex = this->pSplitScreenTex[0];
    if (primaryViewportTex && ((txmManager->state.state >> 15) & 1) != 0)
        return primaryViewportTex->GetSizeX();

    return this->sizeX;
}
