#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiCOORD_GROUP::cbGetLeader @ 0x8322F218
// ?cbGetLeader@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: return the group leader's SSL object reference (an empty ref when there is none).
void aiCOORD_GROUP::cbGetLeader(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    iaIACTOR *leader = GetLeader();

    sslOBJ_REF ref; // empty ref when the group has no leader
    if (leader)
        ref = leader->sslObject;

    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue(asData);
    retVal.StoreValue(tmp);
}
