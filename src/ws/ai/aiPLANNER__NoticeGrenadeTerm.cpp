#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/nav/navPENMAN.h"

// aiPLANNER::NoticeGrenadeTerm(entENTITY* shooter, entENTITY* grenade) @ 0x83164450
// A tracked grenade has terminated: tell every active unit watcher, then the grass awareness
// sub-system, then drop the grenade from the nav penalty manager. (`shooter` is part of the
// virtual signature but the terminate path only needs the grenade.)
void aiPLANNER::NoticeGrenadeTerm(entENTITY *shooter, entENTITY *grenade)
{
    (void)shooter;
    const int nUnits = this->wah.arrUnits.nElem;
    for (int i = 0; i < nUnits; ++i)
    {
        aiWATCHER_BRAIN *watcher = this->wah.PeekActive(i);
        if (watcher)
            watcher->NoticeGrenadeTerm(grenade);
    }

    // aiPLANNER.grass is an embedded aiGRASS (byte-accurate placeholder in the layout header).
    (&this->grass)->NoticeGrenadeTerm(grenade);
    this->penManager->RemoveUnit(grenade);
}
