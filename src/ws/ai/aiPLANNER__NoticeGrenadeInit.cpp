#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/nav/navPENMAN.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3dV.h"

// Zero vector — engine global. boundary.
extern const m3dV m3dVZero;

// ws RTTI helper: nonzero when `addr` dynamically is-a T. The aiBRAIN instantiation is
// ?$gsIs@VaiBRAIN@@...@@YAHPBVmsgADDR@@@Z. boundary.
struct msgADDR;
template <class T> int gsIs(const msgADDR *addr);

// aiPLANNER::NoticeGrenadeInit(entENTITY* shooter, entENTITY* grenade, const m3dV* posEnd) @ 0x831649B8
// Register a newly-thrown grenade: capture its current centre, resolve a landing/target point
// (explicit `posEnd`, else — when the shooter is an aiBRAIN — the shooter's status position), then
// hand it to the nav penalty manager and the grass awareness sub-system.
void aiPLANNER::NoticeGrenadeInit(entENTITY *shooter, entENTITY *grenade, const m3dV *posEnd)
{
    if (!grenade || !shooter)
        return;

    m3dV posGrenade = m3dVZero;
    grenade->GetCenter(&posGrenade);

    m3dV posTarget = m3dVZero;
    if (posEnd)
    {
        posTarget = *posEnd;
    }
    else if (gsIs<aiBRAIN>(reinterpret_cast<const msgADDR *>(shooter)))
    {
        // Shooter is an aiBRAIN: read the position from its status block (aiBRAIN.st @ 0x2CC),
        // whose world-position triple lives at raw byte offset +0x110..+0x118 (aiSTATUS layout is
        // the next frontier — offsets taken from the disassembly, lwz r11,0x2CC(r30) etc.).
        aiBRAIN *brain = reinterpret_cast<aiBRAIN *>(shooter);
        const unsigned char *status = reinterpret_cast<const unsigned char *>(brain->st.pointee);
        posTarget.x = *reinterpret_cast<const float *>(status + 0x110);
        posTarget.y = *reinterpret_cast<const float *>(status + 0x114);
        posTarget.z = *reinterpret_cast<const float *>(status + 0x118);
    }

    this->penManager->AddGrenade(grenade, posTarget);
    // aiGRASS::NoticeGrenadeInit's 4-arg overload takes the two positions by const m3dV& (DB mangling
    // ?NoticeGrenadeInit@aiGRASS@@QAAXPBVentENTITY@@0ABUm3dV@@1@Z — the header decl is correct);
    // pass the locals by reference, not by address.
    (&this->grass)->NoticeGrenadeInit(shooter, grenade, posGrenade, posTarget);
}
