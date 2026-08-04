#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::cbGetTeamName(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83250748
// ?cbGetTeamName@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetTeamName" getter callback: store implGetTeamName's fresh string into `retVal`.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbGetTeamName(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsTSTRING<char> name = implGetTeamName();
    retVal.StoreValue<dsTSTRING<char> >(name);
    // name and the caller ref release themselves at scope end.
}
