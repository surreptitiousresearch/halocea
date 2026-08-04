#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"

// aiWATCHER_PLAYER::IsHOSideLeft @ 0x8328E800
// ?IsHOSideLeft@aiWATCHER_PLAYER@@UBA_NXZ
// Virtual const: true when this player-watcher is validly hidden on the left (outer) side of its
// hideout. The side is encoded directly in fsmPropFlags: the (LEFT|OUTER) bit pair 0xA000 must be
// fully present. Meaningful only while valid and not wall-leaning.
bool aiWATCHER_PLAYER::IsHOSideLeft() const
{
    aiWATCHER_PLAYER *self = const_cast<aiWATCHER_PLAYER *>(this);
    return self->IsValid()
        && !IsWlnMode()
        && (fsmPropFlags.val & 0xA000) == 0xA000; // HO_POS_LEFT | HO_POS_OUTER
}
