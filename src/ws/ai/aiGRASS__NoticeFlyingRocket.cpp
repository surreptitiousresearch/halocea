#include "../../headers/ws/ai/aiGRASS.h"

struct entENTITY;
// ?gssIsPlayer@@YAHPBVentENTITY@@@Z @ 0x829DF450 — true if the entity is a player.  boundary
extern int gssIsPlayer(const entENTITY *ent);

// aiGRASS::NoticeFlyingRocket @ 0x83182948
// A rocket in flight fired by the local player boosts rocket-awareness for the next 2 frames.
void aiGRASS::NoticeFlyingRocket(pjlPJL *rocket, entENTITY *shooter)
{
    (void)rocket;
    if (shooter && gssIsPlayer(shooter))
        this->cntFramesRocket = 2;
}
