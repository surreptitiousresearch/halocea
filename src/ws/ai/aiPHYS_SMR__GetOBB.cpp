#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/ai/aiPHYS_SMR_boundaries.h"  // physRB_BASE::GetPhysOBB
#include "../../headers/ws/anim/animINST_boundary.h"    // animINST::pPhysSystem / pObj / GetOBB
#include "../../headers/ws/anim/objOBJ_boundary.h"      // objOBJ::GetOBB
#include "../../headers/ws/phys/physSYSTEM_BASE.h"       // physSYSTEM_BASE::GetMainBody
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

extern char byte_8200155A[];                             // shared empty-string literal
extern void _apLog(const char *fmt, ...);
objOBJ *objFindName(objOBJ *pObj, const char *name);     // obj/obj_free_functions.h

// aiPHYS_SMR::GetOBB @ 0x832388A0
// ?GetOBB@aiPHYS_SMR@@SA_NPAVanimINST@@AAVm3dOBB@@ABV?$dsTSTRING@D@@@Z (static)
//
// Fill `out` with the oriented bounding box to block against the nav graph. An empty `objName`
// selects the whole instance: the main rigid body's OBB when the instance is physics-driven, else
// the instance's own OBB. A non-empty name resolves a sub-object by name and uses its OBB. The box
// is expanded by 5cm so nav links skimming the surface are reliably blocked. Returns false when the
// instance is null, a named sub-object is not found, or a physics instance has no main body yet.
bool aiPHYS_SMR::GetOBB(animINST *pInst, m3dOBB &out, const dsTSTRING<char> &objName)
{
    if (!pInst)
        return false;

    if (!objName.pBuffer->strLen)
    {
        physSYSTEM_BASE *pPhysSystem = pInst->pPhysSystem;
        if (!pPhysSystem)
        {
            memcpy(&out, pInst->GetOBB(0), sizeof(m3dOBB));
        }
        else
        {
            physRB_BASE *mainBody = pPhysSystem->GetMainBody();
            if (!mainBody)
                return false;
            mainBody->GetPhysOBB(out);
        }
    }
    else
    {
        objOBJ *named = objFindName(pInst->pObj, objName.pBuffer->str);
        if (!named)
        {
            const char *instName = pInst->name ? pInst->name : byte_8200155A;
            _apLog("~AI,Error~Block/Unblock navigation: object '%s' not found in instance '%s'",
                   objName.pBuffer->str, instName);
            return false;
        }
        named->GetOBB(&out);
    }

    out.Expand(0.050000001f);
    return true;
}
