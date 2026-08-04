// aiMIND::SetNormalSelene @ 0x8322BB68
// ?SetNormalSelene@aiMIND@@QAAXXZ
//
// Restore the concrete selene module. If the brain descriptor selects the null selene subsystem,
// defer to ZeroSelene instead. Otherwise, when the current module is missing or is the zero arm,
// install a fresh aiSELENE (copy-and-swap).
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiMIND_arms_boundary.h"
#include "../../headers/ws/ds/ds_new.h"

static const char kMindFile_SNS[] = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_mind.cpp";

void aiMIND::SetNormalSelene()
{
    aiBRAIN      *brain = static_cast<aiBRAIN *>(pBrain);
    aiBRAIN_DESC *desc  = reinterpret_cast<aiBRAIN_DESC *>(brain->spDesc.pointee);

    if (desc->ssTypeSelene == aiBRAIN_DESC::aiSUBSYS_NULL)
    {
        ZeroSelene();
        return;
    }

    aiSELENE_BASE *cur = selene.pointee;
    if (cur && !cur->IsZero())
        return; // already a concrete selene

    aiSELENE *sel = new (kMindFile_SNS, 0x4C4) aiSELENE(brain);

    dsSHARED_PTR<aiSELENE_BASE, 0, Deleter<aiSELENE_BASE> > tmp(sel);
    aiSELENE_BASE *p = selene.pointee; selene.pointee = tmp.pointee; tmp.pointee = p;
    int c = selene.counter; selene.counter = tmp.counter; tmp.counter = c;
    tmp.Reset();
}
