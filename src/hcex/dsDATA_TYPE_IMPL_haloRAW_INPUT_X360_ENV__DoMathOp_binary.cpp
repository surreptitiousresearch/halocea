#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_MATH.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::DoMathOp(binary) @ 0x823BF4F0
// Dispatch a binary math operator (dstTypeId 0) for right->GetTypeId(); undefined if none.
// DEVIATION: mangled return is dsDATA_MATH_RES; the decompiler widened it to int and the
// no-operator path returns -1, i.e. DSD_MATH_UNDEF.
template<>
dsDATA_MATH_RES ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::DoMathOp(
        dsDATA_BINARY_OP op, const unsigned int *left, const unsigned int *right,
        const dsDATA_TYPE *type, dsDATA &ret) const
{
    int typeId = type->GetTypeId();
    dsDATA_MATH_RES (*binaryOp)(const unsigned int *, const unsigned int *, dsDATA &) =
        ds_data::GetBinaryOp(op, 0, typeId);
    if (binaryOp)
        return binaryOp(left, right, ret);
    return DSD_MATH_UNDEF;
}
