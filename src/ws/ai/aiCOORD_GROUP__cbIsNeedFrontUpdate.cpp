#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiCOORD_GROUP::cbIsNeedFrontUpdate @ 0x832322B0
// ?cbIsNeedFrontUpdate@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: unbox the front-name string argument (argv[0]) and dispatch IsNeedFrontUpdate,
// returning the bool result. The manual buffer refcount dec / dlFree in the binary is the inlined
// dsDATA_SIMPLE_WRAPPER<dsTSTRING<char>> destructor.
void aiCOORD_GROUP::cbIsNeedFrontUpdate(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > frontName(argv[0], nullptr);

    bool result = IsNeedFrontUpdate(frontName.val);

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<bool>(result);
    retVal.StoreValue(tmp);
}
