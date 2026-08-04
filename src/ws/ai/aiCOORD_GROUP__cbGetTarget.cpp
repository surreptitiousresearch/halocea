#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiCOORD_GROUP::cbGetTarget @ 0x8322F2E0
// ?cbGetTarget@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: return the group target's SSL object reference (an empty ref when there is none).
void aiCOORD_GROUP::cbGetTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    iaIACTOR *target = GetTarget();

    sslOBJ_REF ref; // empty ref when the group has no target
    if (target)
        ref = target->sslObject;

    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue(asData);
    retVal.StoreValue(tmp);
}
