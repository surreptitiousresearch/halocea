#include "../../headers/vidDRIVER.h"

// 0x8266BA08 -- ?PushRenderTarget@vidDRIVER@@QAAXXZ
// Save the current render-target state by pushing rendTargCur onto the render-target stack.
void vidDRIVER::PushRenderTarget()
{
    this->rendTargStack.PushBack(this->rendTargCur);
}
