#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/prop/propFIREABLE.h"

// aiWATCHER_ENT::IsDead @ 0x8328FC88
// ?IsDead@aiWATCHER_ENT@@UBA_NXZ
// Virtual const: true when the tracked entity's fireable/health component reports a dead or dying
// state. An invalid watcher reports dead; an entity with no fireable component reports not-dead.
// The decompiler inlines iaIACTOR::GetProperty<propFIREABLE> (the container/type-id walk over the
// entity's property + component lists); expressed here as the equivalent GetProperty call.
bool aiWATCHER_ENT::IsDead() const
{
    aiWATCHER_ENT *self = const_cast<aiWATCHER_ENT *>(this); // IsValid is non-const on the base
    if (!self->IsValid())
        return true;

    entENTITY *ent = entity.GetPtr();
    propFIREABLE *fireable = ent->GetProperty<propFIREABLE>();
    if (!fireable)
        return false;

    unsigned int state = fireable->state.state;
    return ((state >> 1) & 1) != 0 || ((state >> 2) & 1) != 0;
}
