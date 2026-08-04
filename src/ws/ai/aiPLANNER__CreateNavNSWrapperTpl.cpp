#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/ds_new.h"
#include "../../headers/ws/ai/aiNAV_NS_WRAPPER_TPL.h"


// aiPLANNER::CreateNavNSWrapperTpl(entENTITY*) const @ 0x83163BB0
// Allocate (tracked, 0xC bytes) and construct a nav-system wrapper bound to this planner's nav
// manager for `ent`; return it (null when the allocation fails).
aiNAV_NS_WRAPPER_TPL *aiPLANNER::CreateNavNSWrapperTpl(entENTITY *ent) const
{
    void *raw = operator new(
        sizeof(aiNAV_NS_WRAPPER_TPL),
        "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_planner.cpp",
        0x1DF);
    aiNAV_NS_WRAPPER_TPL *wrap =
        raw ? new (raw) aiNAV_NS_WRAPPER_TPL(this->navManager, ent) : nullptr;
    return wrap;
}
