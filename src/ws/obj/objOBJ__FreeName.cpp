#include "objOBJ.h"
#include "obj_boundary.h"
// 0x82A7EB80  ?_FreeName@objOBJ@@QAAXXZ
// Release the object name unless bit 0x20 of stateShare marks it externally owned/shared,
// then clear that ownership bit.
// NOTE: the decompiler fabricated phantom params (a2..a6); the disassembly takes only `this`.

void objOBJ::_FreeName()
{
    if ((stateShare.state & 0x20) == 0)
        dlFree(name);
    name = nullptr;
    stateShare.state &= ~0x20;
}
