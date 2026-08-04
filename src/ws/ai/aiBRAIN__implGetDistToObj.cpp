// aiBRAIN::implGetDistToObj @ 0x832AF220
// ?implGetDistToObj@aiBRAIN@@UAAMPAViaIACTOR@@@Z
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ent/entDESC.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

// Distance from this brain to actor `obj`. `obj` must be an entity-derived actor with a live
// animated instance; otherwise an SSL error is logged and a far-distance sentinel (2,000,000) is
// returned.
float aiBRAIN::implGetDistToObj(iaIACTOR *obj)
{
    if (obj)
    {
        dsTYPE_ID typeID = obj->TypeID();
        if (typeID.IsDerivedFrom(entDESC::TYPE_ID))
        {
            // The concrete entity's animated instance lives at actor offset 0x15C (raw — the field is
            // on the actor's dynamic entity type, beyond the iaIACTOR base).
            animINST *inst = *reinterpret_cast<animINST **>(reinterpret_cast<char *>(obj) + 0x15C);
            if (inst)
            {
                m3dV pos;
                inst->GetPos(&pos);
                return Dist(&pos);
            }
        }
    }
    _apLog("~AI,Ssl,Error~'%s' can't call GetDistToObj(): no valid object", names.nameInst);
    return 2000000.0f; // m3d far-distance sentinel (decompiler M3D_INFINITY_825)
}
