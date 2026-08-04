// aiMIND::ZeroSelene @ 0x8322BA98
// ?ZeroSelene@aiMIND@@QAAXXZ
//
// Demote the selene (enemy-selection) module to its no-op "zero" arm, unless it already is one.
// The zero arm is an aiSELENE_IFACE whose vtable pointer is overwritten with aiSELENE_ZERO's, then
// installed (copy-and-swap) in place of the current module.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND_arms_boundary.h"
#include "../../headers/ws/ds/ds_new.h"

static const char kMindFile_ZS[] = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_mind.cpp";

void aiMIND::ZeroSelene()
{
    aiSELENE_BASE *cur = selene.pointee;
    if (cur && cur->IsZero())
        return; // already the zero arm

    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    aiSELENE_IFACE *zero = new (kMindFile_ZS, 0x4B4) aiSELENE_IFACE(brain);
    zero->__vftable = const_cast<void *>(aiSELENE_ZERO::vftable); // re-vtable to the no-op selene

    dsSHARED_PTR<aiSELENE_BASE, 0, Deleter<aiSELENE_BASE> > tmp(zero);
    // copy-and-swap the new module into place; the previous module is released when `tmp` resets
    aiSELENE_BASE *p = selene.pointee; selene.pointee = tmp.pointee; tmp.pointee = p;
    int c = selene.counter; selene.counter = tmp.counter; tmp.counter = c;
    tmp.Reset();
}
