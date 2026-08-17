/* ?GetState@sslSTACK@@QAAXAAUsslSTACK_STATE@@@Z @0x82AF0560 */
#include "sslSTACK.h"
#include "sslSTACK_STATE.h"

// 0x82AF0560 — sslSTACK::GetState. Snapshot the live-element counts of all four sub-stacks so the
// caller can later rewind to this point.
void sslSTACK::GetState(sslSTACK_STATE &state)
{
    state.mainStackSize = mainStack.nElem;
    state.localsStackSize = localsStack.nElem;
    state.foreachStackSize = foreachStack.nElem;
    state.namedParamsPos = namedParams.nElem;
}
