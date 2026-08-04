#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiCOORD_GROUP::cbIsEqualCurves @ 0x832320E8
// ?cbIsEqualCurves@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: compare two named curves (argv[0], argv[1]) within tolerance argv[2]; store the bool.
void aiCOORD_GROUP::cbIsEqualCurves(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    float tolerance = 0.0f;
    (argv + 2)->GetValue<float>(tolerance, DSD_CONV_RETRIEVE);

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > curveB(*(argv + 1), nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > curveA(*argv, nullptr);

    bool result = this->IsEqualCurves(&curveA.val, &curveB.val, tolerance);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
