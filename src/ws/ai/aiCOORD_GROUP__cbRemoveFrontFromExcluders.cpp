#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiCOORD_GROUP::cbRemoveFrontFromExcluders @ 0x83232420
// ?cbRemoveFrontFromExcluders@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: unbox the front-name string argument (argv[0]) and dispatch
// RemoveFrontFromExcluders, returning the bool result.
void aiCOORD_GROUP::cbRemoveFrontFromExcluders(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > frontName(argv[0], nullptr);

    bool result = RemoveFrontFromExcluders(frontName.val);

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<bool>(result);
    retVal.StoreValue(tmp);
}
