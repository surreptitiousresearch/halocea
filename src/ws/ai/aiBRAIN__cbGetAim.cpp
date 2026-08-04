#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::cbGetAim(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83249D88
// ?cbGetAim@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetAim" getter callback: store the brain's current aim name (implGetAim) into `retVal`.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbGetAim(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    retVal.StoreValue<dsTSTRING<char> >(implGetAim());
    // the implGetAim() temporary and the caller ref release themselves at scope end.
}
