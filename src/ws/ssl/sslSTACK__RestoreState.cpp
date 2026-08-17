/* ?RestoreState@sslSTACK@@QAAXABUsslSTACK_STATE@@@Z @0x82AF1B60 */
#include "sslSTACK.h"
#include "sslSTACK_STATE.h"

// 0x82AF1B60 — sslSTACK::RestoreState. Truncate all four sub-stacks back to the live-element counts
// captured in `state` (destroying the elements above each count; no reallocation).
void sslSTACK::RestoreState(const sslSTACK_STATE &state)
{
    mainStack.ShrinkResize(state.mainStackSize);
    localsStack.ShrinkResize(state.localsStackSize);
    foreachStack.ShrinkResize(state.foreachStackSize);
    namedParams.ShrinkResize(state.namedParamsPos);
}
