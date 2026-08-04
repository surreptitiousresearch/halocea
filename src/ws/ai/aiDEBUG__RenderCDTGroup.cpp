// ws-engine ai08 (a8_debug.cpp): aiDEBUG::RenderCDTGroup @0x831999E0 (protected).
// Draw the collision-detection debug gizmos for this brain: the skeleton collision volumes, the
// body CDT shape sphere, the anim CDT cylinder (only when the brain carries game-info), and the
// instance OBB (plain and goal-tinted). Each block is gated by its own con-var toggle.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h" // rendDrv, dbg_isCdt* vars, _pBrainDebugWatchee
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"              // DbgGetMatrShapeCdt / DbgGetRadCdt
#include "../../headers/animINST.h"                        // GetCDTSkeleton / GetOBB / _GetMatrL2W
#include "../../headers/ws/cdt/cdtSKEL.h"                  // cdtSKEL::Render
#include "../../headers/ws/mdl/mdlCDT_SETTINGS.h"          // radCdtAnim / heightCdtAnim
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propCONTAINER_DESC.h"
#include "../../headers/ws/prop/propGAME_INFO.h"
#include "../../headers/ws/prop/propGAME_INFO_DESC.h"
#include "../../headers/ws/m3d/m3d_boundary.h"             // _m3dNormalize
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3dCYL.h"
#include "../../headers/ws/m3d/m3dSPHERE.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/rend/rendDRIVER.h"

void aiDEBUG::RenderCDTGroup()
{
    if (dbg_isCdtSkel.value)
    {
        cdtSKEL *skel = this->pBrain->pInst->GetCDTSkeleton();
        skel->Render(this->pBrain->pInst);
    }

    if (dbg_isCdtGeo.value)
    {
        m3dMATR shapeMatr;
        this->pBrain->body.pointee->DbgGetMatrShapeCdt(shapeMatr);
        m3dSPHERE sphere;
        sphere.org = shapeMatr.v[3];   // translation row = shape centre (decompiler overlaid matr+0x30)
        sphere.radius = this->pBrain->body.pointee->DbgGetRadCdt();
        unsigned int color = this->pBrain->IsDead() ? 0xFF5555FF : 0xFF00CC00;
        rendDrv->RenderSphere(&sphere, color);
    }

    if (dbg_isCdtAnim.value)
    {
        m3dMATR matrL2W;
        this->pBrain->pInst->_GetMatrL2W(&matrL2W);

        // Draw the cylinder only when the brain's container carries a propGAME_INFO (inline
        // GetProperty<propGAME_INFO> walk — same shape as aiBRAIN_IFACE::propGameInfo, no caching).
        propCONTAINER &container = this->pBrain->propContainer;
        bool hasGameInfo;
        if (container.typeId.IsDerivedFrom(propGAME_INFO_DESC::TYPE_ID)) {
            hasGameInfo = true;
        } else if (!container.typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID)) {
            hasGameInfo = false;
        } else {
            hasGameInfo = false;
            for (int i = 0; i < container.properties.nElem; ++i)
                if (container.properties[i]->GetProperty<propGAME_INFO>()) { hasGameInfo = true; break; }
            if (!hasGameInfo)
                for (int i = 0; i < container.components.nElem; ++i)
                    if (container.components[i]->GetProperty<propGAME_INFO>()) { hasGameInfo = true; break; }
        }

        if (hasGameInfo)
        {
            m3dV dirUp = matrL2W.v[1];   // L2W up axis
            _m3dNormalize(&dirUp);
            m3dV base = this->pBrain->GetPos();
            float radCdtAnim = this->pBrain->GetCdtParams()->radCdtAnim;
            float height     = this->pBrain->GetCdtParams()->heightCdtAnim;
            m3dV top = base + dirUp * height;

            m3dCYL cyl;
            cyl.radius = radCdtAnim;
            cyl.to     = base;
            cyl.from   = top;
            rendDrv->RenderCyl(&cyl, 0xFF33FFFF);
        }
    }

    bool highlight = dbg_isHighLightWatchee.value && (this->pBrain == _pBrainDebugWatchee);
    if (dbg_isCdtBBox.value || highlight)
    {
        unsigned int color = 0xFF00FF00;
        if (this->pBrain->IsDead() || (this->pBrain->pInst->state & 1))
            color = 0xFF881111;
        m3dOBB *obb = this->pBrain->pInst->GetOBB(0);
        rendDrv->RenderOBB(obb, color);
    }

    if (dbg_isBBoxGoal.value)
    {
        m3dOBB *obb = this->pBrain->pInst->GetOBB(0);
        rendDrv->RenderOBB(obb, 0xFFFF0000);
    }
}
