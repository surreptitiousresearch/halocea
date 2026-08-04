#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiCOORD_GROUP::cbRemoveNSFromIgnoreList @ 0x83231F20
// ?cbRemoveNSFromIgnoreList@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: remove nav-system argv[0] from the ignore list; store the bool result.
void aiCOORD_GROUP::cbRemoveNSFromIgnoreList(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    // unbox argv[0] into a dsTSTRING<char>; the wrapper releases the string buffer on scope exit.
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);

    bool result = this->RemoveNSFromIgnoreList(&name.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
