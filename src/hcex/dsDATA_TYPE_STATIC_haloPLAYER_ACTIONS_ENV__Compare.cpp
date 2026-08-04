#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/ws/ds/dsDATA_CMP_RES.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::Compare @ 0x823BD9C8
// Look up a comparator (dstTypeId 0) for right->GetTypeId() and run it; undefined if none.
template<>
dsDATA_CMP_RES ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::Compare(
        const unsigned int *left, const unsigned int *right, const dsDATA_TYPE *type)
{
    int typeId = type->GetTypeId();
    dsDATA_CMP_RES (*comparator)(const unsigned int *, const unsigned int *) =
        ds_data::GetComparator(0, typeId);
    if (comparator)
        return comparator(left, right);
    return DSD_CMP_UNDEF;
}
