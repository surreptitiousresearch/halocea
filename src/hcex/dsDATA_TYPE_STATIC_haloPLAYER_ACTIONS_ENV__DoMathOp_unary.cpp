#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsDATA_MATH.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::DoMathOp (unary) @ 0x823BDC00
// Look up a registered unary operator (typeId 0) and run it.
template<>
dsDATA_MATH_RES ds_data::dsDATA_TYPE_STATIC<haloPLAYER_ACTIONS_ENV>::DoMathOp(
        dsDATA_UNARY_OP op, const unsigned int *data, dsDATA &ret)
{
    dsDATA_MATH_RES (*unaryOp)(const unsigned int *, dsDATA &) =
        ds_data::GetUnaryOp(op, 0);
    if (unaryOp)
        return unaryOp(data, ret);
    return DSD_MATH_UNDEF;
}
