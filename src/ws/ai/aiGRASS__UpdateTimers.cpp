#include "../../headers/ws/ai/aiGRASS.h"

// 0x82C3C2F8 (?m3dUpdateTimeField@@YAHPAMM@Z) — count `*field` down by `dt`, clamping at 0. boundary.
extern int m3dUpdateTimeField(float *field, float dt);

// aiGRASS::UpdateTimers @ 0x831834B8
// ?UpdateTimers@aiGRASS@@IAAXM@Z  (protected)
//
// Decay the delay timer of every grenade queued for delayed init by `dt` seconds.
void aiGRASS::UpdateTimers(float dt)
{
    for (ds::LIST<GRENADE_INIT_DATA>::DATA *node = this->grenInits.head; node; node = node->next)
        m3dUpdateTimeField(&node->val.timerDelay, dt);
}
