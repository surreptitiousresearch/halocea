#include "../headers/ws/ds/dsDATA_TYPE_IMPL.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::DoMathOp (unary) @ 0x823BF018
// Virtual override. Mangled return is dsDATA_MATH_RES (decompiler widened to int). Look up a
// unary operator (typeId 0) for `op`; run it, or undefined when none registered.
template<>
dsDATA_MATH_RES ds_data::dsDATA_TYPE_IMPL<haloPLAYER_ACTIONS_ENV>::DoMathOp(
        dsDATA_UNARY_OP op, const unsigned int *data, dsDATA &ret) const
{
    dsDATA_MATH_RES (*unaryOp)(const unsigned int *, dsDATA &) =
        ds_data::GetUnaryOp(op, 0);
    if (unaryOp)
        return unaryOp(data, ret);
    return DSD_MATH_UNDEF;
}
