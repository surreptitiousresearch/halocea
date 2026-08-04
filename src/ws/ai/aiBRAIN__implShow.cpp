#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ent/entENTITY.h"  // base entENTITY::implShow
#include "../../headers/ws/msg/msgADDR.h"     // suspendState + OnChangeMode vtable slot

// aiBRAIN::implShow() @ 0x832AD8C8
// ?implShow@aiBRAIN@@UAAXXZ
//
// Virtual show: run the base entity show, then (if the low suspend bit is currently set) clear it
// via the message-system OnChangeMode transition (old state -> old state & ~1).
void aiBRAIN::implShow()
{
    entENTITY::implShow();
    if ((suspendState & 1) != 0)
        __vftable->OnChangeMode(this, suspendState, suspendState & 0xFFFFFFFE);
}
