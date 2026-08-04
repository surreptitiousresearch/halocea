#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3dSPHERE.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/obj/obj_free_functions.h"  // objFindName
#include "../../headers/ws/anim/anim_validate_boundary.h" // objCalcLTM

// aiDEBUG::RenderCoordSysGroup @ 0x83197CF0  (?RenderCoordSysGroup@aiDEBUG@@IAAXXZ)
//
// Draw the enabled coordinate-system debug gizmos for this brain: the L2W instance CS, the nav CS,
// the nav-association marker sphere, the status inst/body coordinate systems, the model ROOT CS,
// and every matrix cached in csList. Each block is gated by its own con-var toggle.
void aiDEBUG::RenderCoordSysGroup()
{
    if (dbg_isPosCS.value)
    {
        m3dMATR m;
        this->pBrain->pInst->GetMatrL2W(&m);
        // Translate's 4th `oper` arg was dropped by the decompiler; the gizmo is nudged +0.1 in Y.
        m.Translate(0.0f, 0.1f, 0.0f, 0);
        rendDrv->RenderCS(&m, 2.0f);
    }

    if (dbg_isNavCS.value)
    {
        m3dMATR m;
        this->pBrain->GetBodyMatrNav(m);
        rendDrv->RenderCS(&m, 2.0f);
    }

    if (dbg_isNavAssocCS.value)
    {
        m3dMATR m;
        this->pBrain->GetBodyMatrNav(m);
        m3dV up = m3dVUnitY * 0.25f;
        m3dV pos;
        m.TransformPoint(&up, &pos);
        m3dSPHERE sphere;
        sphere.radius = 0.050000001f;
        sphere.org = pos;
        rendDrv->RenderSphere(&sphere, 0xFFFF0000);
    }

    if (dbg_isInstCS.value)
    {
        const aiSTATUS::CS &cs = this->pBrain->st.pointee->cs;
        m3dV org = cs.posInst;
        _m3dCheckValid(&org);
        _m3dCheckValid(&cs.dirInst);
        m3dV fwd   = { cs.dirInst.x + org.x, cs.dirInst.y + org.y, cs.dirInst.z + org.z };
        _m3dCheckValid(&org);
        _m3dCheckValid(&cs.dirUpInst);
        m3dV up    = { cs.dirUpInst.x + org.x, cs.dirUpInst.y + org.y, cs.dirUpInst.z + org.z };
        _m3dCheckValid(&org);
        _m3dCheckValid(&cs.dirRightInst);
        m3dV right = { cs.dirRightInst.x + org.x, cs.dirRightInst.y + org.y, cs.dirRightInst.z + org.z };
        rendDrv->RenderLine(&org, &fwd,   0xFF0000FF);
        rendDrv->RenderLine(&org, &up,    0xFF00FF00);
        rendDrv->RenderLine(&org, &right, 0xFFFF0000);
    }

    if (dbg_isCSStatus.value)
    {
        const aiSTATUS::CS &cs = this->pBrain->st.pointee->cs;
        m3dV org = cs.posBody;
        _m3dCheckValid(&org);
        _m3dCheckValid(&cs.dirBody);
        m3dV fwd   = { cs.dirBody.x + org.x, cs.dirBody.y + org.y, cs.dirBody.z + org.z };
        _m3dCheckValid(&org);
        _m3dCheckValid(&cs.dirUpBody);
        m3dV up    = { cs.dirUpBody.x + org.x, cs.dirUpBody.y + org.y, cs.dirUpBody.z + org.z };
        _m3dCheckValid(&org);
        _m3dCheckValid(&cs.dirRightBody);
        m3dV right = { cs.dirRightBody.x + org.x, cs.dirRightBody.y + org.y, cs.dirRightBody.z + org.z };
        rendDrv->RenderLine(&org, &fwd,   0xFF0000FF);
        rendDrv->RenderLine(&org, &up,    0xFF00FF00);
        rendDrv->RenderLine(&org, &right, 0xFFFF0000);
    }

    if (dbg_isRootCS.value)
    {
        objOBJ *root = objFindName(this->pBrain->pInst->pObj, "ROOT");
        if (root)
        {
            objCalcLTM(root);
            rendDrv->RenderCS(&root->matrLT, 1.0f);
        }
    }

    if (dbg_isListCS.value)
    {
        for (int i = 0; i < this->csList.nElem; ++i)
            rendDrv->RenderCS(&this->csList[i], 0.60000002f);
    }
}
