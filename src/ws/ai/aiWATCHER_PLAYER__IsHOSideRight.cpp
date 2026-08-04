#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"

// aiWATCHER_PLAYER::IsHOSideRight @ 0x8328E8A0
// ?IsHOSideRight@aiWATCHER_PLAYER@@UBA_NXZ
// Virtual const: true when this player-watcher is validly hidden on the right (outer) side of its
// hideout. The (RIGHT|OUTER) bit pair 0xC000 must be fully present in fsmPropFlags. Meaningful only
// while valid and not wall-leaning.
bool aiWATCHER_PLAYER::IsHOSideRight() const
{
    aiWATCHER_PLAYER *self = const_cast<aiWATCHER_PLAYER *>(this);
    return self->IsValid()
        && !IsWlnMode()
        && (fsmPropFlags.val & 0xC000) == 0xC000; // HO_POS_RIGHT | HO_POS_OUTER
}
