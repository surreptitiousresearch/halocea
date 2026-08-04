#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entSFX_LIST.h"

// entENTITY::GetSfx @ 0x8270BD80 — return the idx-th live sound-effect instance from this entity's
// sfx list, or nullptr when idx is out of range (bounds-checked against sfxArray.nElem).
entSFX *entENTITY::GetSfx(int idx) const
{
    const entSFX_LIST *list = &this->sfxList;
    if (idx < 0 || idx >= list->sfxArray.nElem)
        return nullptr;
    return list->sfxArray[idx];
}
