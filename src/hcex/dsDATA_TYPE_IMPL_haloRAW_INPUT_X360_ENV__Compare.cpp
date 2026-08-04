#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_CMP_RES.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Compare @ 0x823BF2F0
// Look up a comparator (dstTypeId 0) for type->GetTypeId() and run it; undefined if none.
// DEVIATION: mangled return is dsDATA_CMP_RES; the decompiler widened it to int and the
// no-comparator path returns -1, i.e. DSD_CMP_UNDEF.
template<>
dsDATA_CMP_RES ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::Compare(
        const unsigned int *left, const unsigned int *right, const dsDATA_TYPE *type) const
{
    int typeId = type->GetTypeId();
    dsDATA_CMP_RES (*comparator)(const unsigned int *, const unsigned int *) =
        ds_data::GetComparator(0, typeId);
    if (comparator)
        return comparator(left, right);
    return DSD_CMP_UNDEF;
}
