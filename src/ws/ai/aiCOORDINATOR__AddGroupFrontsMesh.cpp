#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiDOM_NAV_boundary.h"  // aiDOM_NAV::pObj
#include "../../headers/ws/ai/aiPLANNER.h"            // aiPLANNER::GetNavManager
#include "../../headers/ws/nav/navSYS.h"              // navSYS::IsWpSetReady
#include "../../headers/ws/nav/navMANAGER.h"          // navMANAGER::GetNavDomain
#include "../../headers/ws/anim/objOBJ_boundary.h"    // objOBJ::GetVert / GetFace / nVert / nFace
#include "../../headers/ws/obj/objFACE.h"
#include "../../headers/ws/m3d/m3dV.h"

// The ai08 planner singleton (aiEXIFACE.h).
extern aiPLANNER *aiPlanner;

// aiCOORDINATOR::AddGroupFrontsMesh @ 0x83237008
// ?AddGroupFrontsMesh@aiCOORDINATOR@@IAAXPBVnavSYS@@ABV?$dsTSTRING@D@@KAAV?$dsVECTOR@UaiDBGSHAPE_VERT@@$07@@AAV?$dsVECTOR@G$07@@@Z
//
// Append the nav-domain geometry of a single named front to the given debug vertex/index buffers,
// tinted `color`. Only runs when the front's WP-set is loaded and the front resolves to a nav
// domain with a model object. Face indices are rebased by the buffer's current vertex count.
void aiCOORDINATOR::AddGroupFrontsMesh(const navSYS *dbgUsedNS, const dsTSTRING<char> &frontName,
                                       unsigned long color, dsVECTOR<aiDBGSHAPE_VERT, 8> &vBuf,
                                       dsVECTOR<unsigned short, 8> &iBuf)
{
    if (!dbgUsedNS->IsWpSetReady(frontName))
        return;

    aiDOM_NAV *navDomain = aiPlanner->GetNavManager()->GetNavDomain(frontName);
    if (!navDomain)
        return;

    objOBJ *pObj = navDomain->pObj;
    if (!pObj)
        return;

    int baseVert = vBuf.nElem;

    int nVert = pObj->nVert;
    aiDBGSHAPE_VERT vert;
    vert.color = color;
    for (int i = 0; i < nVert; ++i)
    {
        m3dV pos;
        pObj->GetVert(i, &pos);
        vert.v = pos;
        vBuf.PushBack(vert);
    }

    int nFace = pObj->nFace;
    for (int i = 0; i < nFace; ++i)
    {
        objFACE *face = pObj->GetFace(i);
        if (face)
        {
            unsigned short idx;
            idx = static_cast<unsigned short>(face->ind[0] + baseVert); iBuf.PushBack(idx);
            idx = static_cast<unsigned short>(face->ind[1] + baseVert); iBuf.PushBack(idx);
            idx = static_cast<unsigned short>(face->ind[2] + baseVert); iBuf.PushBack(idx);
        }
    }
}
