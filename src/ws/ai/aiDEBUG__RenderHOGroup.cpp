#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/m3d/m3dSPHERE.h"

// aiDEBUG::RenderHOGroup @ 0x83194A28  (?RenderHOGroup@aiDEBUG@@IAAXXZ)
//
// When the enter-hideout position-penalty debug var is on and a penalty was recorded this frame,
// draw a small wire sphere at the recorded penalty centre.
void aiDEBUG::RenderHOGroup()
{
    if (dbg_isHOPosPenalty.value && this->havePenaltyEnterHO)
    {
        m3dSPHERE sphere;
        sphere.org = this->posCenterPenaltyEnterHO;
        sphere.radius = 0.050000001f;
        rendDrv->RenderSphere(&sphere, 0xFFu);
    }
}
