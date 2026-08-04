#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/nav/navPENMAN.h"
#include "../../headers/ws/ent/entENTITY.h"

// aiPLANNER::implNoticeGrenadeExplosion @ 0x83185278
// A tracked grenade entity has detonated. Notify every active watcher-brain so it can drop its
// grenade tracking, remove the grenade from the grass awareness system, and clear its nav penalty.
// The virtual is declared over iaIACTOR* but the runtime object is always an entENTITY (all callees
// take entENTITY*), matching the disassembly's straight reinterpretation.
void aiPLANNER::implNoticeGrenadeExplosion(iaIACTOR *actor)
{
    entENTITY *grenade = static_cast<entENTITY *>(actor);

    int count = this->wah.arrUnits.nElem;
    for (int i = 0; i < count; ++i)
    {
        aiWATCHER_BRAIN *unit = this->wah.PeekActive(i);
        if (unit)
            unit->NoticeGrenadeTerm(grenade);
    }

    (&this->grass)->RemoveFlyingGrenade(grenade);
    this->penManager->RemoveUnit(grenade);
}
