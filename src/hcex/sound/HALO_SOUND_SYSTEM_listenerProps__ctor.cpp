#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

// HALO_SOUND_SYSTEM::<unnamed_type_listenerProps>::listenerProps_t() @ 0x836B5580
// Default ctor for the nested listener-transform snapshot. m3dV/m3dMATR have no member
// constructors reversed in this batch, so nothing is initialized here -- matches the empty
// decompiled body verbatim (the fields hold whatever garbage was already on the stack until
// the first UpdateListener-style write, which is not part of this class per the DB).
HALO_SOUND_SYSTEM::listenerProps_t::listenerProps_t()
{
}
