#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiCOORD_GROUP::cbIsTargetValid @ 0x8322FF48
// ?cbIsTargetValid@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: report whether the group's current target is valid.
// NOTE: IsTargetValid is a no-argument virtual (?IsTargetValid@aiCOORD_GROUP@@UAA_NXZ); the
// decompiler's (this, argc, argv) shape reflects stale registers, not real args.
void aiCOORD_GROUP::cbIsTargetValid(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    bool result = IsTargetValid();

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<bool>(result);
    retVal.StoreValue(tmp);
}
