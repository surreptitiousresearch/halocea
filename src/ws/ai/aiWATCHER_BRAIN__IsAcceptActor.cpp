#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"

// aiWATCHER_BRAIN::IsAcceptActor(iaIACTOR*) @ 0x8316EE68  (static)
// A brain-watcher only accepts actors whose runtime type derives from aiBRAIN_DESC (i.e. the
// actor is an AI brain). The runtime type-id is obtained via the actor's msgADDR vtbl slot 0.

// ai08 — behavior-descriptor RTTI anchor for aiBRAIN. boundary (its TYPE_ID is defined out of line).
struct aiBRAIN_DESC { static const dsTYPE_ID TYPE_ID; };

bool aiWATCHER_BRAIN::IsAcceptActor(iaIACTOR *actor)
{
    dsTYPE_ID typeId;
    actor->__vftable->TypeID(actor, &typeId);
    return typeId.IsDerivedFrom(aiBRAIN_DESC::TYPE_ID) != 0;
}
