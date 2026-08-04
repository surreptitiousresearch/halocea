#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h" // DSD_CONV_RETRIEVE

// aiCOORD_GROUP::cbGetMember @ 0x8322FBB8
// ?cbGetMember@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: fetch member at index argv[0] and return its SSL object reference.
void aiCOORD_GROUP::cbGetMember(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    int index = 0;
    argv->GetValue<int>(index, DSD_CONV_RETRIEVE);

    iaIACTOR *member = GetMember(index);

    sslOBJ_REF ref; // empty ref when the member is null
    if (member)
        ref = member->sslObject;

    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue(asData);
    retVal.StoreValue(tmp);
}
