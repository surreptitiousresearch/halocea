#include "../../headers/ws/ai/aiAPPROACH.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3dV.h"
// animINST (with GetPos) comes from entENTITY.h -> anim/animINST.h; do not also include
// animINST_boundary.h (it re-defines the struct and would collide).

// aiAPPROACH::CheckDistance(const entENTITY *) @ 0x8329A700
// ?CheckDistance@aiAPPROACH@@IBA_NPBVentENTITY@@@Z
//
// True when `entity` exists, has a model instance, and its instance origin lies within `radius`
// of the owning brain.
bool aiAPPROACH::CheckDistance(const entENTITY *entity) const
{
    if (!entity)
        return false;

    animINST *inst = entity->pInst;
    if (!inst)
        return false;

    m3dV pos;
    inst->GetPos(&pos);
    return pBrain->Dist(&pos) < radius;
}
