#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global SSL system instance

// aiCOORD_GROUP::cbAddMember @ 0x8322F840
// ?cbAddMember@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: resolve argv[0] to an actor via the SSL system and add it to the group; return
// whether it was added.
void aiCOORD_GROUP::cbAddMember(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    sslOBJ_REF ref;
    ref.Set(*argv, gsSslSystem); // resolves the actor reference; returned sslERROR is discarded

    iaIACTOR *member = (iaIACTOR *)ref.GetCbHost();
    bool added = AddMember(member);

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<bool>(added);
    retVal.StoreValue(tmp);
}
