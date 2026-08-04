#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiPLANNER::cbFindGroupByName(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8325A398
// ?cbFindGroupByName@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "FindGroupByName" callback: unbox argv[0] (group name), forward to implFindGroupByName, box the
// returned object reference into `retVal`. `caller` releases on return.
void aiPLANNER::cbFindGroupByName(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > grpName(argv[0], nullptr);

    sslOBJ_REF group = implFindGroupByName(grpName.val);

    dsDATA out = group;        // sslOBJ_REF::operator dsDATA — adopt the ref into a dsDATA value
    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue(out);
    retVal.StoreValue(boxed);
    // group, out, boxed, and grpName destructors self-release at scope end.
}
