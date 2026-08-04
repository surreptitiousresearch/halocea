#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::cbGetBHV(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324BE60
// ?cbGetBHV@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetBHV" getter callback: store the brain's current behaviour name (implGetBHV) into `retVal`.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbGetBHV(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    retVal.StoreValue<dsTSTRING<char> >(implGetBHV());
    // the implGetBHV() temporary and the caller ref release themselves at scope end.
}
