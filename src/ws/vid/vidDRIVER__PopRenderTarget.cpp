/* ?PopRenderTarget@vidDRIVER@@QAAHXZ @0x8266B3A8 */
#include "../../headers/vidDRIVER.h"

// 0x8266B3A8 -- ?PopRenderTarget@vidDRIVER@@QAAHXZ
// Restore the render-target state saved by the most recent PushRenderTarget. If the stack is
// empty, unbind to the default target (SetRenderTarget(null, null, 3)). Otherwise re-apply the
// top-of-stack entry through the virtual SetRenderTarget(const vidRT_STACK_ENTRY *) slot and pop
// it. Returns the value produced by the applied SetRenderTarget.
int vidDRIVER::PopRenderTarget()
{
    if (!this->rendTargStack.nElem)
        return this->SetRenderTarget(nullptr, nullptr, apSTATE_T<unsigned long>{3});

    int result = this->SetRenderTarget(&this->rendTargStack.Back());

    int nElem = this->rendTargStack.nElem;
    if (nElem)
        this->rendTargStack.ShrinkResize(nElem - 1);

    return result;
}
