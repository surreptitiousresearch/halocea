#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ent/entENTITY.h"  // base entENTITY::implHide
#include "../../headers/ws/msg/msgADDR.h"     // suspendState + OnChangeMode vtable slot

// aiBRAIN::implHide() @ 0x832AD870
// ?implHide@aiBRAIN@@UAAXXZ
//
// Virtual hide: run the base entity hide, then (if the low suspend bit is not already set) set it
// via the message-system OnChangeMode transition (old state -> old state | 1).
void aiBRAIN::implHide()
{
    entENTITY::implHide();
    if ((suspendState & 1) == 0)
        __vftable->OnChangeMode(this, suspendState, suspendState | 1);
}
