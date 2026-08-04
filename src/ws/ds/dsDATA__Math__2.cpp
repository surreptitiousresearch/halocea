#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsDATA_MATH.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_assert_boundary.h"

// dsDATA::Math (binary) @ 0x82AD7B98
// Combine this value with `right` under binary operator `op`, writing the result into `ret`.
// First dispatch this type's binary DoMathOp with the operands as-is. If that returns
// DSD_MATH_UNDEF (this type has no op for these operand types), retry twice by promoting one
// operand to the other's type via a DSD_CONV_MATH conversion:
//   1. construct a scratch of THIS type, convert `right` into it, dispatch this-type op;
//   2. failing that, construct a scratch of RIGHT's type, convert THIS into it, dispatch right-type op.
// Returns the math result (DSD_MATH_UNDEF if no path produced one). Operands are passed as raw
// storage pointers (dsDATA::storage@0, so `this`/`&right` double as their storage).
dsDATA_MATH_RES dsDATA::Math(dsDATA_BINARY_OP op, const dsDATA &right, dsDATA &ret) const
{
    if (!IGNORE_STRONG_ASSERT && !this->type)
        STRONG_ASSERT_DUMMY().Crash(
            "type != 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_data_impl.hpp",
            389,
            dsStrongAssertMessage);

    dsDATA_MATH_RES res = this->type->DoMathOp(
        op, (const unsigned int *)this, (const unsigned int *)&right, right.type, &ret);
    if (res != DSD_MATH_UNDEF)
        return res;

    // No direct operator: promote `right` into THIS's type and retry with this-type dispatch.
    unsigned int scratch[16]; // 64-byte value staging area
    this->type->Construct(scratch);
    if (right.type->Convert((const unsigned int *)&right, scratch, this->type, DSD_CONV_MATH)) {
        dsDATA_MATH_RES thisRes = this->type->DoMathOp(
            op, (const unsigned int *)this, scratch, this->type, &ret);
        if (thisRes != DSD_MATH_UNDEF) {
            this->type->Destroy(scratch);
            return thisRes;
        }
    }
    this->type->Destroy(scratch);

    // Still nothing: promote THIS into RIGHT's type and retry with right-type dispatch.
    right.type->Construct(scratch);
    if (this->type->Convert((const unsigned int *)this, scratch, right.type, DSD_CONV_MATH)) {
        dsDATA_MATH_RES rightRes = right.type->DoMathOp(
            op, scratch, (const unsigned int *)&right, right.type, &ret);
        if (rightRes != DSD_MATH_UNDEF) {
            right.type->Destroy(scratch);
            return rightRes;
        }
    }
    right.type->Destroy(scratch);
    return DSD_MATH_UNDEF;
}
