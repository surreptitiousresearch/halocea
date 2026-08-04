#include "../headers/ws/ds/ds_boundary.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/ws/ds/dsDATA_MATH.h"
#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::DoMathOp (binary) @ 0x823BE030
// Look up a binary operator (dstTypeId 0) for right->GetTypeId() and run it; undefined if none.
template<>
dsDATA_MATH_RES ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::DoMathOp(
        dsDATA_BINARY_OP op, const unsigned int *left, const unsigned int *right,
        const dsDATA_TYPE *type, dsDATA &ret)
{
    int typeId = type->GetTypeId();
    dsDATA_MATH_RES (*binaryOp)(const unsigned int *, const unsigned int *, dsDATA &) =
        ds_data::GetBinaryOp(op, 0, typeId);
    if (binaryOp)
        return binaryOp(left, right, ret);
    return DSD_MATH_UNDEF;
}

// ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::DoMathOp (unary) @ 0x823BE0B8
// Look up a unary operator (dstTypeId 0) and run it; undefined if none.
template<>
dsDATA_MATH_RES ds_data::dsDATA_TYPE_STATIC<haloRAW_INPUT_X360_ENV>::DoMathOp(
        dsDATA_UNARY_OP op, const unsigned int *data, dsDATA &ret)
{
    dsDATA_MATH_RES (*unaryOp)(const unsigned int *, dsDATA &) = ds_data::GetUnaryOp(op, 0);
    if (unaryOp)
        return unaryOp(data, ret);
    return DSD_MATH_UNDEF;
}
