#include "../../headers/ws/prop/propBASE.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propCONTAINER_DESC.h"
#include "../../headers/ws/prop/propHALO_OBJ.h"
#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"

// propBASE::GetProperty<propHALO_OBJ> @ 0x823D83E0
// (??$GetProperty@VpropHALO_OBJ@@@propBASE@@QBAPAVpropHALO_OBJ@@XZ)
// Runtime-type probe: if this prop itself derives from propHALO_OBJ_DESC, return it as a
// propHALO_OBJ*. Otherwise, only when this is actually a property container (derives from
// propCONTAINER_DESC), recurse through its properties then its components, returning the first
// match. This is the target of the hcex bridge's flattened externs propBASE_GetProperty_HALO_OBJ
// and propBASE_GetProperty_propHALO_OBJ (both resolve here).
template<> propHALO_OBJ *propBASE::GetProperty<propHALO_OBJ>()
{
    if (this->typeId.IsDerivedFrom(propHALO_OBJ_DESC::TYPE_ID))
        return (propHALO_OBJ *)this;

    if (!this->typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID))
        return nullptr;

    propCONTAINER *container = (propCONTAINER *)this;

    for (int i = 0; i < container->properties.nElem; ++i)
    {
        propHALO_OBJ *found = container->properties[i]->GetProperty<propHALO_OBJ>();
        if (found)
            return found;
    }

    for (int i = 0; i < container->components.nElem; ++i)
    {
        propHALO_OBJ *found = container->components[i]->GetProperty<propHALO_OBJ>();
        if (found)
            return found;
    }

    return nullptr;
}
