#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiCOORD_GROUP::cbSetAngleFnCurve @ 0x83232060
// ?cbSetAngleFnCurve@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: select the angle-function curve argv[0] by name.
void aiCOORD_GROUP::cbSetAngleFnCurve(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)retVal; (void)caller;

    // unbox argv[0] into a dsTSTRING<char>; the wrapper releases the string buffer on scope exit.
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);

    this->SetAngleFnCurve(&name.val);
}
