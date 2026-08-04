#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbFailBHVByNameAndStackAbove(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832573C0
// ?cbFailBHVByNameAndStackAbove@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "FailBHVByNameAndStackAbove" callback: unbox the behaviour-name string out of argv[0],
// evaluate the virtual implFailBHVByNameAndStackAbove and box its bool result into `retVal`.
// `caller` self-releases on return.
void aiBRAIN::cbFailBHVByNameAndStackAbove(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > bhvName(*argv, nullptr);

    bool result = implFailBHVByNameAndStackAbove(bhvName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
