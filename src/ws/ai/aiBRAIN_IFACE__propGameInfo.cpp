// ws-engine ai08 (a8_ext_iface.cpp): aiBRAIN_IFACE::propGameInfo @0x83193940 (non-const).
// Lazily resolve (and cache) this actor's propGAME_INFO property. Same shape as
// iaIACTOR::GetProperty<propGAME_INFO> with a cached slot + a strong-assert on failure:
//   - if the actor's propContainer IS itself a game-info container, use it directly;
//   - else, only when it owns a property collection, scan properties[] then components[].
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propCONTAINER_DESC.h"
#include "../../headers/ws/prop/propGAME_INFO.h"
#include "../../headers/ws/prop/propGAME_INFO_DESC.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;      // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A;   // shared empty-string / assert info byte @0x8200155A

propGAME_INFO *aiBRAIN_IFACE::propGameInfo()
{
    if (__propGameInfo.val)
        return __propGameInfo.val;

    propCONTAINER &container = propContainer;
    propGAME_INFO *result = nullptr;

    if (container.typeId.IsDerivedFrom(propGAME_INFO_DESC::TYPE_ID)) {
        result = reinterpret_cast<propGAME_INFO *>(&container);
    } else if (container.typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID)) {
        for (int i = 0; i < container.properties.nElem; ++i) {
            result = container.properties[i]->GetProperty<propGAME_INFO>();
            if (result)
                break;
        }
        if (!result) {
            for (int i = 0; i < container.components.nElem; ++i) {
                result = container.components[i]->GetProperty<propGAME_INFO>();
                if (result)
                    break;
            }
        }
    }

    __propGameInfo.val = result;
    if (!IGNORE_STRONG_ASSERT && !result)
        STRONG_ASSERT_DUMMY::Crash(
            nullptr, "__propGameInfo",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ext_iface.cpp", 26,
            byte_8200155A);
    return __propGameInfo.val;
}
