#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiCOORD_GROUP::cbGetMembersCount @ 0x8322FB28
// ?cbGetMembersCount@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: report the number of members in the group.
// NOTE: GetMembersCount is a no-argument virtual (?GetMembersCount@aiCOORD_GROUP@@UAAHXZ); the
// decompiler's (this, argc, argv) shape reflects stale registers, not real args.
void aiCOORD_GROUP::cbGetMembersCount(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    int result = GetMembersCount();

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<int>(result);
    retVal.StoreValue(tmp);
}
