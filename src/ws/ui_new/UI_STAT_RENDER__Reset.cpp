#include "../../headers/ws/ui_new/UI_STAT_RENDER.h"

// 0x82CED900 — drop the batched quad geometry back to empty (only once a texture is bound; an
// un-InitTexture'd renderer has nothing to reset).
void ui_new::UI_STAT_RENDER::Reset()
{
    if (pTex.ptr)
    {
        iBuf.nElem = 0;
        vBuf.nElem = 0;
    }
}
