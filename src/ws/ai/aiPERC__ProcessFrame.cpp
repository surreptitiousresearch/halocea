#include "../../headers/ws/ai/aiPERC.h"

// aiPERC::ProcessFrame @ 0x8329D760
// ?ProcessFrame@aiPERC@@UAAXM@Z
//
// Per-frame perception update: refresh the radar, then run grenade-avoidance, victim-damage
// reporting, and the war/peace decision.
void aiPERC::ProcessFrame(float dt)
{
    UpdateRadar(dt);                // virtual (vtbl slot 15); body external to this batch
    UpdateGrenadeAvoidance();
    UpdateVictimDamaged();
    UpdateWarPeace();
}
