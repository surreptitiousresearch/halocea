#include "../../headers/ws/ai/aiDIE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"

extern dbgVAR_IMPL<bool, 1> dbgDropItemsAfterDeath; // ai08 debug gate @ 0x84326FB8
extern void _apLog(const char *fmt, ...);           // engine formatted log sink — boundary

// aiDIE::DropWeaponAndItems @ 0x832AA7B0
// ?DropWeaponAndItems@aiDIE@@UAAXXZ
//
// When the drop-items-after-death debug gate is enabled and the brain has not already dropped
// (AI_DIEST bit 0x4000 clear), log the event and throw the brain's held weapons/items.
void aiDIE::DropWeaponAndItems()
{
    if (dbgDropItemsAfterDeath.value)
    {
        aiBRAIN *brain = pBrain;
        if (((brain->st.pointee->die.val >> 14) & 1) == 0)
        {
            _apLog("~AI,Dbgout~'%s': died and dropped weapon", brain->names.nameInst);
            pBrain->implThrowWeaponsAndItems();
        }
    }
}
