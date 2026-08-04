#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA_MATH.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::DoMathOp(unary) @ 0x823BF588
// Dispatch a unary math operator (typeId 0) for this null/untyped instance; undefined if none.
// DEVIATION: mangled return is dsDATA_MATH_RES; the decompiler widened it to int and the
// no-operator path returns -1, i.e. DSD_MATH_UNDEF.
template<>
dsDATA_MATH_RES ds_data::dsDATA_TYPE_IMPL<haloRAW_INPUT_X360_ENV>::DoMathOp(
        dsDATA_UNARY_OP op, const unsigned int *data, dsDATA &ret) const
{
    dsDATA_MATH_RES (*unaryOp)(const unsigned int *, dsDATA &) =
        ds_data::GetUnaryOp(op, 0);
    if (unaryOp)
        return unaryOp(data, ret);
    return DSD_MATH_UNDEF;
}
