#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_CMP_RES.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Compare @ 0x823BED80
// Virtual override. Mangled return is dsDATA_CMP_RES (decompiler widened it to int). Look up a
// comparator (dstTypeId 0) for right->GetTypeId() and run it; undefined when none registered.
template<>
dsDATA_CMP_RES ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::Compare(
        const unsigned int *left, const unsigned int *right, const dsDATA_TYPE *type) const
{
    int typeId = type->GetTypeId();
    dsDATA_CMP_RES (*comparator)(const unsigned int *, const unsigned int *) =
        ds_data::GetComparator(0, typeId);
    if (comparator)
        return comparator(left, right);
    return DSD_CMP_UNDEF;
}
