#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiCOORD_GROUP::cbGetTimeTargetInHO @ 0x83230068
// ?cbGetTimeTargetInHO@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: report how long the group's target has been in a hideout (seconds, float).
// NOTE: GetTimeTargetInHO is a no-argument virtual (?GetTimeTargetInHO@aiCOORD_GROUP@@UAAMXZ);
// the decompiler's (this, argc, argv) shape reflects stale registers, not real args.
void aiCOORD_GROUP::cbGetTimeTargetInHO(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    float result = GetTimeTargetInHO();

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<float>(result);
    retVal.StoreValue(tmp);
}
