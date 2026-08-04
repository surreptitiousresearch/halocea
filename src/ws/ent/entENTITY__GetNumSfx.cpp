#include "../../headers/ws/ent/entENTITY.h"

// entENTITY::GetNumSfx @ 0x827068A8
// Number of live sound-effect instances tracked by this entity.
int entENTITY::GetNumSfx() const
{
    return sfxList.sfxArray.nElem;
}
