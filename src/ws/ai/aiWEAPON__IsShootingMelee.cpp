#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_TIE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/ds/dsSTRID_FLAGS.h"

// aiWEAPON::IsShootingMelee @ 0x83295CE8
// ?IsShootingMelee@aiWEAPON@@UBA_NXZ  (virtual, const)
//
// True when the currently selected strike is a melee strike (its filter accepts the wpnS_DESC::MELEE
// property) and the module is actively firing right now.
bool aiWEAPON::IsShootingMelee() const
{
    if (tieSelected.IsNull())
        return false;

    // One-time-constructed melee property set (the binary's guarded function-local static).
    static const dsSTRID_FLAGS meleeFlags(wpnS_DESC::MELEE);

    return tieSelected.desc->filter.Execute(meleeFlags) && IsShootingNow();
}
