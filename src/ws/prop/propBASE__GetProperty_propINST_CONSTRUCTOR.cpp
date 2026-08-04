#include "../../headers/ws/prop/propBASE.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propCONTAINER_DESC.h"
#include "../../headers/ws/prop/propINST_CONSTRUCTOR.h"

// propBASE::GetProperty<propINST_CONSTRUCTOR> @ 0x823D82E8
// (??$GetProperty@VpropINST_CONSTRUCTOR@@@propBASE@@QBAPAVpropINST_CONSTRUCTOR@@XZ)
// Identical shape to the propHALO_OBJ instantiation: return this cast to propINST_CONSTRUCTOR* if
// it derives from propINST_CONSTRUCTOR_DESC, else recurse through a container's properties then
// components. Target of the hcex flattened extern propBASE_GetProperty_INST_CONSTRUCTOR.
template<> propINST_CONSTRUCTOR *propBASE::GetProperty<propINST_CONSTRUCTOR>()
{
    if (this->typeId.IsDerivedFrom(propINST_CONSTRUCTOR_DESC::TYPE_ID))
        return (propINST_CONSTRUCTOR *)this;

    if (!this->typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID))
        return nullptr;

    propCONTAINER *container = (propCONTAINER *)this;

    for (int i = 0; i < container->properties.nElem; ++i)
    {
        propINST_CONSTRUCTOR *found = container->properties[i]->GetProperty<propINST_CONSTRUCTOR>();
        if (found)
            return found;
    }

    for (int i = 0; i < container->components.nElem; ++i)
    {
        propINST_CONSTRUCTOR *found = container->components[i]->GetProperty<propINST_CONSTRUCTOR>();
        if (found)
            return found;
    }

    return nullptr;
}
