#include "../../headers/vidDRIVER.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

// @0x82668D70 -- ?UnsetRenderTarget@vidDRIVER@@QAAXPAVtxmTEXTURE@@0@Z
// Public convenience wrapper: build a render-target stack entry naming the color/depth textures to
// unbind (def-use state fixed to 3) and apply it through the virtual
// UnsetRenderTarget(const vidRT_STACK_ENTRY *) slot.
void vidDRIVER::UnsetRenderTarget(txmTEXTURE *pTexRend, txmTEXTURE *pTexDepth)
{
    vidRT_STACK_ENTRY entry;
    entry.state[0].state = 3;
    entry.state[1].state = 0;
    entry.pTexRendTarg[0] = pTexRend;
    entry.pTexRendTarg[1] = nullptr;
    entry.pTexRendTargZS = pTexDepth;
    entry.sx = 0;
    entry.sy = 0;

    this->UnsetRenderTarget(&entry);
}
