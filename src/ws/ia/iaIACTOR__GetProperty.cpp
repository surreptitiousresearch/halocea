// iaIACTOR::GetProperty<P>() — reversed for P = propHALO_OBJ (@0x823DB164), propINST_CONSTRUCTOR
// (@0x823DB06C), propOBJ_MODIFIER_LIST (@0x823DFC6C). Identical shape for all three: if
// propContainer.typeId itself derives from P's DESC::TYPE_ID, this actor's propContainer IS the
// property (reinterpreted as P*). Otherwise, only when propContainer.typeId derives from
// propCONTAINER_DESC::TYPE_ID (this actor really owns a property collection), linear-scan
// properties[] then components[] calling each entry's propBASE::GetProperty<P>(), returning the
// first non-null hit.
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/prop/propHALO_OBJ.h"
#include "../../headers/ws/prop/propINST_CONSTRUCTOR.h"
#include "../../headers/ws/prop/propOBJ_MODIFIER_LIST.h"
#include "../../headers/ws/dsc/dscDESC.h"

template<class P>
static P *iaIACTOR_GetProperty_Impl(iaIACTOR *self, const dsTYPE_ID &leafTypeId, const dsTYPE_ID &containerTypeId)
{
    propCONTAINER &container = self->propContainer;

    if (container.typeId.IsDerivedFrom(leafTypeId))
        return (P *)&container;

    if (!container.typeId.IsDerivedFrom(containerTypeId))
        return nullptr;

    for (int i = 0; i < container.properties.nElem; ++i) {
        P *found = container.properties[i]->GetProperty<P>();
        if (found)
            return found;
    }
    for (int i = 0; i < container.components.nElem; ++i) {
        P *found = container.components[i]->GetProperty<P>();
        if (found)
            return found;
    }
    return nullptr;
}

template<>
propHALO_OBJ *iaIACTOR::GetProperty<propHALO_OBJ>()
{
    return iaIACTOR_GetProperty_Impl<propHALO_OBJ>(this, propHALO_OBJ_DESC::TYPE_ID, propCONTAINER_DESC::TYPE_ID);
}

template<>
propINST_CONSTRUCTOR *iaIACTOR::GetProperty<propINST_CONSTRUCTOR>()
{
    return iaIACTOR_GetProperty_Impl<propINST_CONSTRUCTOR>(this, propINST_CONSTRUCTOR_DESC::TYPE_ID, propCONTAINER_DESC::TYPE_ID);
}

template<>
propOBJ_MODIFIER_LIST *iaIACTOR::GetProperty<propOBJ_MODIFIER_LIST>()
{
    return iaIACTOR_GetProperty_Impl<propOBJ_MODIFIER_LIST>(this, propOBJ_MODIFIER_LIST_DESC::TYPE_ID, propCONTAINER_DESC::TYPE_ID);
}
