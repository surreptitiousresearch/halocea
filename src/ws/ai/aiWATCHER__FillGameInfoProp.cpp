#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propCONTAINER_DESC.h"
#include "../../headers/ws/prop/propGAME_INFO.h"
#include "../../headers/ws/prop/propGAME_INFO_DESC.h"

// aiWATCHER::FillGameInfoProp @ 0x8328F930
// ?FillGameInfoProp@aiWATCHER@@IAAXXZ
//
// Lazily resolve and cache the watched entity's propGAME_INFO. No-op once cached. When the entity's
// property container is itself a game-info container it is used directly; otherwise (a generic
// container) its properties then components are scanned for the first propGAME_INFO. Caches the
// result (possibly null) so the scan runs at most once per entity.
void aiWATCHER::FillGameInfoProp()
{
    if (this->pGameInfo)
        return;

    entENTITY *ent = this->GetEntity();
    if (!ent) {
        this->pGameInfo = nullptr;
        return;
    }

    propCONTAINER &container = ent->propContainer;
    propGAME_INFO *result = nullptr;

    if (container.typeId.IsDerivedFrom(propGAME_INFO_DESC::TYPE_ID)) {
        this->pGameInfo = reinterpret_cast<propGAME_INFO *>(&container);
        return;
    }

    if (container.typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID)) {
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

    this->pGameInfo = result;
}
