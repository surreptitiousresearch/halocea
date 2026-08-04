#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // aiPERC_BASE::NoticeGrenade

// aiGRASS::SendGrenadesDanger @ 0x83183780
// ?SendGrenadesDanger@aiGRASS@@QBAXPAVaiBRAIN@@@Z  (const)
//
// Notify the brain's perception module about every grenade currently tracked in flight.
void aiGRASS::SendGrenadesDanger(aiBRAIN *pBrain) const
{
    for (ds::LIST<GRENADE_INFO>::DATA *node = this->grenades.head; node; node = node->next)
        pBrain->mind.perc.pointee->NoticeGrenade(
            const_cast<entENTITY *>(reinterpret_cast<const entENTITY *>(node->val.grenade)));
}
