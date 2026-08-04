#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_TIE.h"
#include "../../headers/ws/wpn/wpnWEAPON_boundary.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiWEAPON::SwitchLaser @ 0x83294668
// ?SwitchLaser@aiWEAPON@@IAAX_N@Z
//
// Turn the selected weapon's "pointer" laser sound-effect on or off, if the owning entity has one.
void aiWEAPON::SwitchLaser(bool enable)
{
    wpnWEAPON *iface = GetIFace(tieSelected);
    if (!iface)
        return;

    // pOwner is canonically iaIACTOR * (propBASE); entENTITY derives from it, so downcast.
    entENTITY *owner = static_cast<entENTITY *>(iface->pOwner);
    if (!owner->GetSfx("pointer"))
        return;

    dsTSTRING<char> name("pointer");
    if (enable)
        owner->implStartSfx(name);
    else
        owner->implStopSfx(name);
}
