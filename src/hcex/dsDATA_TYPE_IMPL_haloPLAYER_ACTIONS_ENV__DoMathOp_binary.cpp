#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::DoMathOp (binary) @ 0x823BEF80
// Virtual override. Mangled return is dsDATA_MATH_RES (decompiler widened to int). Look up a
// binary operator (dstTypeId 0) for `op` and right->GetTypeId(); run it, or undefined when none.
template<>
dsDATA_MATH_RES ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::DoMathOp(
        dsDATA_BINARY_OP op, const unsigned int *left, const unsigned int *right,
        const dsDATA_TYPE *type, dsDATA &ret) const
{
    int rightTypeId = type->GetTypeId();
    dsDATA_MATH_RES (*binaryOp)(const unsigned int *, const unsigned int *, dsDATA &) =
        ds_data::GetBinaryOp(op, 0, rightTypeId);
    if (binaryOp)
        return binaryOp(left, right, ret);
    return DSD_MATH_UNDEF;
}
