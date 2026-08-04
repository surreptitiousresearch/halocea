#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiGRASS::NoticeGrenadeInit(const entENTITY* shooter, const entENTITY* grenade,
//                            const m3dV& posStart, const m3dV& posEnd) @ 0x831850D0
// Track the grenade (start/end positions given). If the shooter's dynamic type derives from
// aiBRAIN_DESC, additionally start hideout smoke-out handling for it. (The decompiler's manual
// weak-ref decrement after PushBack is the tracked temporary's GRENADE_INFO destructor — scope exit.)
void aiGRASS::NoticeGrenadeInit(const entENTITY *shooter, const entENTITY *grenade,
                                const m3dV &posStart, const m3dV &posEnd)
{
    {
        GRENADE_INFO info(shooter, grenade, posStart, posEnd);
        this->grenades.PushBack(info);
    }

    dsTYPE_ID typeId = shooter->TypeID();
    if (typeId.IsDerivedFrom(aiBRAIN_DESC::TYPE_ID))
        this->HandleSmokingOut(static_cast<const aiBRAIN *>(shooter));
}
