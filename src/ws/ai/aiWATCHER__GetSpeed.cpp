#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // canonical m3dVZero

// aiWATCHER::GetSpeed @ 0x8328DA28
// ?GetSpeed@aiWATCHER@@UBA?AUm3dV@@XZ
// Virtual const: the tracked entity's world-space velocity; m3dVZero when the watcher is no longer
// valid.
m3dV aiWATCHER::GetSpeed() const
{
    aiWATCHER *self = const_cast<aiWATCHER *>(this); // IsValid/GetEntity are non-const on the base

    if (!self->IsValid())
        return m3dVZero;

    m3dV speed;
    self->GetEntity()->GetSpeed(&speed);
    return speed;
}
