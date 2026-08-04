#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/ds_new.h"
#include "../../headers/ws/ai/aiNAV_WP_WRAPPER_TPL.h"


// aiPLANNER::CreateNavWPWrapperTpl(entENTITY*) const @ 0x83163B48
// Allocate (tracked, 0x10 bytes) and construct a nav waypoint wrapper bound to this planner's
// nav manager for `ent`; return it (null when the allocation fails).
aiNAV_WP_WRAPPER_TPL *aiPLANNER::CreateNavWPWrapperTpl(entENTITY *ent) const
{
    void *raw = operator new(
        sizeof(aiNAV_WP_WRAPPER_TPL),
        "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_planner.cpp",
        0x1DB);
    aiNAV_WP_WRAPPER_TPL *wrap =
        raw ? new (raw) aiNAV_WP_WRAPPER_TPL(this->navManager, ent) : nullptr;
    return wrap;
}
