#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/dmg/dmgLOG.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiPROP_FIREABLE::AcceptDmgLog(const dmgDAMAGE*) @ 0x832A3958
// ?AcceptDmgLog@aiPROP_FIREABLE@@MAAXPBVdmgDAMAGE@@@Z (protected virtual)
// Open a scoped "Damage_AI" damage-log entry for this hit (starting health snapshot) and record it.
void aiPROP_FIREABLE::AcceptDmgLog(const dmgDAMAGE *pDmg)
{
    int healthStart = this->Hp();
    aiBRAIN *owner = (aiBRAIN *)this->pOwner; // pOwner is concretely an aiBRAIN

    dsSTRID nameFilter("Damage_AI", false);
    dmgLOG log(owner->pInst, healthStart, nameFilter);
    log.Start(pDmg);
    // ~dmgLOG closes the entry at scope exit
}
