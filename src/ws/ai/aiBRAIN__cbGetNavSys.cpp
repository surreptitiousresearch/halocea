#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::cbGetNavSys(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324D2D8
// ?cbGetNavSys@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetNavSys" getter callback: store the name of the brain's nav-system (implGetNavSys) into
// `retVal`. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbGetNavSys(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    retVal.StoreValue<dsTSTRING<char> >(implGetNavSys());
    // the implGetNavSys() temporary and the caller ref release themselves at scope end.
}
