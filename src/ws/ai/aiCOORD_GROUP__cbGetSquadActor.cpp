#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiCOORD_GROUP::cbGetSquadActor @ 0x83232648
// ?cbGetSquadActor@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: unbox the squad-actor-name string argument (argv[0]), look up the actor via
// GetSquadActor, and return its SSL object reference (an empty ref when there is no such actor).
void aiCOORD_GROUP::cbGetSquadActor(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > squadActorName(argv[0], nullptr);

    iaIACTOR *actor = GetSquadActor(squadActorName.val);

    sslOBJ_REF ref; // empty ref when there is no matching squad actor
    if (actor)
        ref = actor->sslObject;

    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue(asData);
    retVal.StoreValue(tmp);
}
