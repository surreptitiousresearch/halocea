#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/ws/ds/dsDATA_MATH.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::DoMathOp (binary) @ 0x823BDB78
// Look up a registered binary operator (dstTypeId 0) for right->GetTypeId() and run it.
template<>
dsDATA_MATH_RES ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::DoMathOp(
        dsDATA_BINARY_OP op, const unsigned int *left, const unsigned int *right,
        const dsDATA_TYPE *type, dsDATA &ret)
{
    int srcTypeId = type->GetTypeId();
    dsDATA_MATH_RES (*binaryOp)(const unsigned int *, const unsigned int *, dsDATA &) =
        ds_data::GetBinaryOp(op, 0, srcTypeId);
    if (binaryOp)
        return binaryOp(left, right, ret);
    return DSD_MATH_UNDEF;
}
