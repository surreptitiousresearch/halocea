#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbIsEnemyType(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256708
// ?cbIsEnemyType@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "IsEnemyType" getter callback: unbox the type-name string out of argv[0], evaluate the
// virtual implIsEnemyType and store the bool result into `retVal`. `caller` self-releases.
void aiBRAIN::cbIsEnemyType(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);

    bool result = implIsEnemyType(name.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
