#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsDATA_MATH.h"
#include "ds_assert_boundary.h"

// dsDATA::Math @ 0x82A41B40
// Apply unary operator `op` to this value in place: dispatch the stored type's DoMathOp into a
// scratch dsDATA; if it produced a result, adopt that value; then tear down the scratch.
dsDATA_MATH_RES dsDATA::Math(dsDATA_UNARY_OP op)
{
    if (!IGNORE_STRONG_ASSERT && !this->type)
        // Crash is invoked on a null this-pointer by the STRONG_ASSERT macro.
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "type != 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_data_impl.hpp",
            371,
            empty_string);

    dsDATA scratch;
    scratch.type = nullptr;
    dsDATA_MATH_RES res = this->type->DoMathOp(op, this, &scratch);
    if (res == DSD_MATH_OK)
        this->SetValue(&scratch);
    if (scratch.type)
        scratch.type->Destroy(&scratch.storage);
    return res;
}
