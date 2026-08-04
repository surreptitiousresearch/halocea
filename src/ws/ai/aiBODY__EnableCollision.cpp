#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/mdl/mdlINFO_BASE.h"
#include "../../headers/ws/cdt/cdtREFINE.h"

// 0x832AB918 ?EnableCollision@aiBODY@@UAAX_N@Z
// Toggle the body's sphere-refine collision. Bit 0x80 in the refine state = "collision disabled":
// when `enable` is requested it is cleared, otherwise set. No-op when the refine bit already matches.
void aiBODY::EnableCollision(bool enable)
{
    mdlINFO_BASE *info = this->infoBodyMdl;
    if (!info)
        return;

    cdtREFINE *refine = info->GetSphereRefine();
    if (!refine)
        return;

    unsigned int state = refine->state.state;
    if ((((~state) >> 7) & 1) != (unsigned int)enable)
    {
        unsigned int newState = state | 0x80u;
        if (enable)
            newState = refine->state.state & 0xFFFFFF7Fu;
        refine->state.state = newState;
    }
}
