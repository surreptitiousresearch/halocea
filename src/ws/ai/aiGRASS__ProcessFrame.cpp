#include "../../headers/ws/ai/aiGRASS.h"

// aiGRASS::ProcessFrame @ 0x83184D78
// Per-frame update of the grenade/rocket awareness system.
void aiGRASS::ProcessFrame(float dt)
{
    this->ProcessSmokingOut();

    if (this->grenInits.length > 0)
    {
        // The queued-grenade drain body is inlined away in the decompiler output (the traversal
        // survives as an empty loop over grenInits); reproduced faithfully as a no-op walk.
        for (ds::LIST<GRENADE_INIT_DATA>::DATA *i = this->grenInits.head; i; i = i->next)
        {
        }
    }

    this->SendNotificationsToNPC();

    if (this->cntFramesRocket > 0)
        --this->cntFramesRocket;

    this->UpdateTimers(dt);
}
