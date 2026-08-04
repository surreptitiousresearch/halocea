#include "../../headers/ws/ai/aiGRASS.h"

// aiGRASS::AddFlyingGrenadeForInit @ 0x83183818
// ?AddFlyingGrenadeForInit@aiGRASS@@IAAXPBVentENTITY@@0ABUm3dV@@@Z  (protected)
//
// Queue a grenade for delayed initialisation: record thrower, grenade and launch acceleration,
// arm a short delay timer, and append it to the grenInits list.
void aiGRASS::AddFlyingGrenadeForInit(const entENTITY *shooter, const entENTITY *grenade,
                                      const m3dV &accel)
{
    GRENADE_INIT_DATA data;
    data.shooter    = shooter;
    data.grenade    = grenade;
    data.accel.x    = accel.x;
    data.timerDelay = 0.0099999998f; // ~1 tick delay before registering the throw
    data.accel.y    = accel.y;
    data.accel.z    = accel.z;
    this->grenInits.PushBack(data);
}
