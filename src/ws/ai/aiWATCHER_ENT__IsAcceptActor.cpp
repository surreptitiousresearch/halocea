#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ent/entDESC.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"

// aiWATCHER_ENT::IsAcceptActor(iaIACTOR*) @ 0x8328EBA0  (static)
// ?IsAcceptActor@aiWATCHER_ENT@@SA_NPAViaIACTOR@@@Z
// An entity-watcher only accepts actors whose runtime type derives from entDESC (i.e. the actor
// is a game entity). The runtime type-id is obtained through the actor's virtual TypeID() (vtbl
// slot 0), then tested against the entity-descriptor RTTI anchor.
bool aiWATCHER_ENT::IsAcceptActor(iaIACTOR *actor)
{
    dsTYPE_ID typeId = actor->TypeID();
    return typeId.IsDerivedFrom(entDESC::TYPE_ID) != 0;
}
