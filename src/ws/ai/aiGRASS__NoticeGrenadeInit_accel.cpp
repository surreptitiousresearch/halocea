#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3dV.h"

// Zero vector — engine global. boundary.
extern const m3dV m3dVZero;

// aiGRASS::NoticeGrenadeInit(const entENTITY* shooter, const entENTITY* grenade, const m3dV& accel)
// @ 0x83185188
// Register a grenade given only by its shooter/acceleration. When the shooter is an aiBRAIN
// (derives from aiBRAIN_DESC), resolve the start point from the grenade's current centre and the
// end point from the shooter's status-block world position (aiSTATUS +0x110..+0x118, the same raw
// offsets used by aiPLANNER::NoticeGrenadeInit — aiSTATUS is the next frontier), then delegate to
// the explicit-position overload. Otherwise queue it for delayed init.
void aiGRASS::NoticeGrenadeInit(const entENTITY *shooter, const entENTITY *grenade, const m3dV &accel)
{
    if (!shooter || !grenade)
        return;

    dsTYPE_ID typeId = shooter->TypeID();
    if (typeId.IsDerivedFrom(aiBRAIN_DESC::TYPE_ID))
    {
        m3dV posGrenade = m3dVZero;
        // GetCenter is a non-const virtual; the ABI passes `grenade` as const here.
        const_cast<entENTITY *>(grenade)->GetCenter(&posGrenade);

        const aiBRAIN *brain = static_cast<const aiBRAIN *>(shooter);
        const unsigned char *status = reinterpret_cast<const unsigned char *>(brain->st.pointee);
        m3dV posStatus;
        posStatus.x = *reinterpret_cast<const float *>(status + 0x110);
        posStatus.y = *reinterpret_cast<const float *>(status + 0x114);
        posStatus.z = *reinterpret_cast<const float *>(status + 0x118);

        this->NoticeGrenadeInit(shooter, grenade, posGrenade, posStatus);
    }
    else
    {
        this->AddFlyingGrenadeForInit(shooter, grenade, accel);
    }
}
