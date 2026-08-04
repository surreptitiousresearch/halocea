#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiGRASS::AddFlyingGrenade @ 0x83184A10
// Append a freshly-constructed GRENADE_INFO for this grenade to the tracked-grenades list.
// (The decompiler's trailing osLockedDecrement/operator-delete is the inlined GRENADE_INFO
// destructor releasing the temporary's shooter weak reference — expressed here as scope exit.)
void aiGRASS::AddFlyingGrenade(const entENTITY *shooter, const entENTITY *grenade,
                               const m3dV &posStart, const m3dV &posEnd)
{
    GRENADE_INFO info(shooter, grenade, posStart, posEnd);
    this->grenades.PushBack(info);
}
